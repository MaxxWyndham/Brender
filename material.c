#include "brender.h"
#include "material.h"

br_material* BrMaterialAllocate(const char* name)
{
	br_material *material = BrResAllocate(NULL, sizeof(br_material), BR_MEMORY_MATERIAL);

	if (name != NULL)
        material->identifier = BrResStrDup(material, name);

	material->flags = 0;
	material->index_shade = NULL;
	material->index_blend = NULL;
	material->colour_map = NULL;
	material->extra_prim = NULL;
	material->user = NULL;

	return material;
}

br_material* BrDefaultMaterial()
{
	br_material *m 	= BrMaterialAllocate("default");
	m->colour 		= BR_COLOUR_RGBA(255, 255, 255, 255);
	m->ka			= BR_UFRACTION(0.10);
	m->kd			= BR_UFRACTION(0.70);
	m->ks			= BR_UFRACTION(0.0);
	m->power		= BR_SCALAR(20);
	m->flags		= BR_MATF_LIGHT;
	m->index_base	= 0;
	m->index_range	= 63;
	BrMatrix23Identity(&m->map_transform);

	return m;
}

void BrMaterialUpdate(br_material* material, br_uint_16 flags)
{
	//if (flags != BR_MATU_ALL) { INFO("%u", flags); }

	if (flags == BR_MATU_ALL || (flags & BR_MATU_MAP_TRANSFORM)) {}
	if (flags == BR_MATU_ALL || (flags & BR_MATU_RENDERING)) {}
	if (flags == BR_MATU_ALL || (flags & BR_MATU_LIGHTING)) {}
	if (flags == BR_MATU_ALL || (flags & BR_MATU_COLOURMAP)) {}
}

br_uint_32 BrMaterialEnum(const char* pattern, br_material_enum_cbfn* callback, void* arg)
{
	WARN("Ignoring pattern (\"%s\") processing everything", pattern);
	br_node *mats = reg_materials->head;

	while (mats)
	{
		callback((br_material *)mats->data, arg);
		mats = mats->next;
	}

	return 0;
}

br_uint_32 BrMaterialFileCount(const char* filename, br_uint_16* num)
{
    *num = 0;
    br_uint_32 s = BrFileGetLength(filename);
    br_uint_32 ret = 1;

    if (s > 16)
    {
        FILE *fp;
        fp = BrFileOpenRead(filename, 0, NULL, 0);
        char *buffer_start = BrMemCalloc(s, sizeof(char), BR_MEMORY_DATAFILE);
        char *buffer = buffer_start;
        BrFileRead(buffer, sizeof(char), s, fp);
        BrFileClose(fp);

        /* magic numbers are magic */
        if (BrMemReadBU32(&buffer) == 18 &&
            BrMemReadBU32(&buffer) == 8  &&
            BrMemReadBU32(&buffer) == 5  &&
            BrMemReadBU32(&buffer) == 2)
        {
            br_uint_32 pn = (br_uint_32)buffer + s - 16;
            br_uint_32 blocktype;
            br_uint_32 blocksize;

            while ((br_uint_32)buffer < pn)
            {
                blocktype = BrMemReadBU32(&buffer);
                blocksize = BrMemReadBU32(&buffer);

                switch (blocktype)
                {
                    case 4:
                        *num += 1;
                        BrMemSkipBytes(&buffer, 48);
                        BrMemReadString(&buffer);
                        break;

                    case 28:
                        BrMemReadString(&buffer);
                        break;

                    case 31:
                        BrMemReadString(&buffer);
                        break;

                    case 0:
                        BrMemSkipBytes(&buffer, blocksize);
                        break;

                    default:
                        INFO("%s:",filename);
                        INFO("unknown block %u (len %u) @ %u", blocktype, blocksize, (br_uint_32)buffer);
                        break;
                }
            }

            ret = 0;
        }

        BrMemFree(buffer_start);
        buffer_start = NULL;
        buffer = NULL;
    }

    return ret;
}

br_material* BrMaterialLoad(const char* filename)
{
    return BrMaterialAllocate(NULL);

    br_material *m;

    if (BrMaterialLoadMany(filename, &m, 1))
    {
        return m;
    }
    else
    {
        WARN("%s missing or corrupt", filename);
        return NULL;
    }
}

br_uint_32 BrMaterialLoadMany(const char* filename, br_material** materials, br_uint_16 num)
{
	if (BrMaterialFileCount(filename, &num) > 0) { return 0; }

    if (num > 0)
    {
        br_uint_32 s = BrFileGetLength(filename);

        if (s)
        {
            FILE *fp;
            fp = BrFileOpenRead(filename, 0, NULL, 0);
            BrFileAdvance(16, fp); // First 16 bytes are magic number
            s -= 16;

            char *buffer_start = BrMemCalloc(s, sizeof(char), BR_MEMORY_DATAFILE);
            char *buffer = buffer_start;
            BrFileRead(buffer, sizeof(char), s, fp);
            BrFileClose(fp);

            br_uint_32 pn = (br_uint_32)buffer + s;
            br_uint_32 blocktype;
            br_uint_32 blocksize;
            br_uint_32 i = 0;

            while ((br_uint_32)buffer < pn)
            {
                blocktype = BrMemReadBU32(&buffer);
                blocksize = BrMemReadBU32(&buffer);

                switch (blocktype)
                {
                    case 4:
                        BrMemSkipBytes(&buffer, 48);
                        materials[i] = BrMaterialAllocate(BrMemReadString(&buffer));
                        break;

                    case 28:
                        materials[i]->colour_map = BrMapFind(BrMemReadString(&buffer));
                        break;

                    case 31:
                        materials[i]->index_shade = BrTableFind(BrMemReadString(&buffer));
                        break;

                    case 0:
                        i++;
                        break;

                    default:
                        INFO("%s:",filename);
                        INFO("unknown block %u (len %u) @ %u", blocktype, blocksize, (br_uint_32)buffer);
                        break;
                }
            }

            BrMemFree(buffer_start);
            buffer_start = NULL;
            buffer = NULL;
        }
    }

    return num;
}

br_material* BrMaterialAdd(br_material* material)
{
	BrMaterialUpdate(material, BR_MATU_ALL);
	BrListInsert(reg_materials, material);
	return material;
}

br_uint_32 BrMaterialAddMany(br_material* const* materials, int n)
{
	br_uint_32 added = 0;

	for (int i = 0; i < n; i++)
		if (BrMaterialAdd(materials[i]))
			added++;

	return added;
}

br_material* BrMaterialFind(const char* pattern)
{
	return (br_material *)BrListFind(reg_materials, pattern);
}

br_material* BrMaterialRemove(br_material* material)
{
	return (br_material *)BrListRemove(reg_materials, material);
}

void BrMaterialFree(br_material* m)
{
	BrMemFree(m->identifier);
	BrMemFree(m);
}
