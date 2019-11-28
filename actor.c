#include "brender.h"
#include "actor.h"

br_actor *BrActorAllocate(br_uint_8 actor_type, void* type_data)
{
	br_actor *actor = BrResAllocate(NULL, sizeof(br_actor), BR_MEMORY_ACTOR);
	actor->type = actor_type;

	actor->parent = NULL;
	actor->next = NULL;
	actor->prev = NULL;
	actor->children = NULL;
	actor->depth = 0;
	actor->model = DEFAULT_MODEL;
	actor->material = DEFAULT_MATERIAL;

	if (type_data == NULL)
	{
		switch (actor_type)
		{
			case BR_ACTOR_NONE:
			case BR_ACTOR_MODEL:
				break;

			case BR_ACTOR_CAMERA:
			{
				br_camera* camera = BrResAllocate(actor, sizeof(br_camera), BR_MEMORY_CAMERA);
				camera->type			= BR_CAMERA_PERSPECTIVE;
				camera->field_of_view	= BR_ANGLE_DEG(45);
				camera->hither_z		= BR_SCALAR(0.1);
				camera->yon_z			= BR_SCALAR(10.0);
				camera->aspect			= BR_SCALAR(1.0);

				actor->type_data = camera;
				//INFO("Created new br_camera");
				break;
			}

			case BR_ACTOR_LIGHT:
			{
				br_light* light = BrResAllocate(actor, sizeof(br_light), BR_MEMORY_LIGHT);
				light->type				= BR_LIGHT_DIRECT;
				light->colour			= BR_COLOUR_RGB(255, 255, 255);
				light->attenuation_c	= BR_SCALAR(1.0);

				actor->type_data = light;
				//INFO("Created new br_light");
				break;
			}

            case BR_ACTOR_BOUNDS_CORRECT:
            {
				br_bounds* bounds = BrResAllocate(actor, sizeof(br_bounds), BR_MEMORY_BOUNDS);
				actor->type_data = bounds;
				//INFO("Created new br_bounds");
				break;
            }

            case BR_ACTOR_CLIP_PLANE:
            {
				br_vector4* clip = BrResAllocate(actor, sizeof(br_vector4), BR_MEMORY_CLIP_PLANE);
				actor->type_data = clip;
				//INFO("Created new clip plane (br_vector4)");
				break;
            }

			default:
				WARN("No allocation code for: %d", actor_type);
				break;
		}
	}
	else
	{
		actor->type_data = type_data;
	}

	actor->t.type = BR_TRANSFORM_MATRIX34;
	BrMatrix34Identity(&actor->t.t.mat);

	return actor;
}

br_actor* BrActorAdd(br_actor* parent, br_actor* a)
{
	a->depth = parent->depth + 1;
	a->parent = parent;
	a->next = NULL;
	a->prev = NULL;

	if (parent->children) {
		br_actor *p = parent->children;

		while (p->next)
		{
			//INFO("%p->%s", p, p->identifier);
			p = p->next;
		}

		p->next = a;
		a->prev = p;
	} else {
		//a->prev = parent;
		parent->children = a;
	}

	return a;
}

br_actor* BrActorRemove(br_actor* a)
{
	if (a->parent->children == a) { a->parent->children = a->next; }
	if (a->prev) { ((br_actor *)(a->prev))->next = a->next;	}
	if (a->next) { a->next->prev = a->prev;	}

	a->parent = NULL;
	a->prev = NULL;
	a->next = NULL;

	return a;
}

br_uint_32 BrActorEnum(br_actor* parent, br_actor_enum_cbfn* callback, void* arg)
{
	br_intptr_t result;
	br_actor *pActor = parent->children;

	while (pActor)
	{
		if ((result = callback(pActor, arg)))
			return (result);

		pActor = pActor->next;
	}
	return (0);
}

br_bounds* BrActorToBounds(br_bounds* b, const br_actor* ap)
{
	const br_actor *p = ap;

	while (p)
	{
		if (p->type == BR_ACTOR_MODEL)
		{
			br_model *model = p->model;

			b->min.v[0] = BR_MIN(b->min.v[0], model->bounds.min.v[0]);
			b->min.v[1] = BR_MIN(b->min.v[1], model->bounds.min.v[1]);
			b->min.v[2] = BR_MIN(b->min.v[2], model->bounds.min.v[2]);
			b->max.v[0] = BR_MAX(b->max.v[0], model->bounds.max.v[0]);
			b->max.v[1] = BR_MAX(b->max.v[1], model->bounds.max.v[1]);
			b->max.v[2] = BR_MAX(b->max.v[2], model->bounds.max.v[2]);
		}
		else if (p->type == BR_ACTOR_BOUNDS || p->type == BR_ACTOR_BOUNDS_CORRECT)
		{
			br_bounds *bounds = (br_bounds *)p->type_data;

			b->min.v[0] = BR_MIN(b->min.v[0], bounds->min.v[0]);
			b->min.v[1] = BR_MIN(b->min.v[1], bounds->min.v[1]);
			b->min.v[2] = BR_MIN(b->min.v[2], bounds->min.v[2]);
			b->max.v[0] = BR_MAX(b->max.v[0], bounds->max.v[0]);
			b->max.v[1] = BR_MAX(b->max.v[1], bounds->max.v[1]);
			b->max.v[2] = BR_MAX(b->max.v[2], bounds->max.v[2]);
		}

		if (p->children) { BrActorToBounds(b, p->children); }

		p = p->next;
	}

	return b;
}

br_uint_8 BrActorToActorMatrix34(br_matrix34* m, const br_actor* a, const br_actor* b)
{
	BrMatrix34Identity(m);

	while (a)
	{
		BrMatrix34Post(m, &a->t.t.mat);
		a = a->parent;
		if (a == b) { break; }
	}

	return BR_TRANSFORM_MATRIX34;
}

void BrActorRelink(br_actor* parent, br_actor* a)
{
	FIXME("(%p, %p): stub", parent, a);
//	a->depth = parent->depth + 1;
//	a->parent = parent;
//	a->prev = NULL;
//	a->next = NULL;
//
//	if (parent->children) {
//		br_actor *p = parent->children;
//
//		while (p->next)
//			p = p->next;
//
//		p->next = a;
//		a->prev = &p->next;
//	} else {
//		a->prev = &parent->children;
//		parent->children = a;
//	}
//
//	if (a->children)
//	{
//		br_actor *p = a->children;
//
//		while (p->next)
//		{
//			p = p->next;
//			INFO("Relinking children");
//			BrActorRelink(a, p);
//		}
//	}
}

void BrActorFree(br_actor* a)
{
	if (a == NULL)
		ERR("param is NULL!");

	if (a->children)
	{
		br_actor *children = a->children;
		BrActorRemove(children);
		BrActorFree(children);
	}
//
	if (a->identifier != NULL) { BrMemFree(a->identifier); }
//	if (a->type_data != NULL) { BrMemFree(a->type_data); }
//	if (a->model != NULL) { BrModelFree(a->model); }
//
	BrMemFree(a);
}

br_uint_32 BrActorSave(const char* filename, const br_actor* actor)
{
	FIXME("(\"%s\", %p): stub", filename, actor);
	return 0;
}

br_uint_32 BrActorFileCount(const char* filename, br_uint_16* num)
{
    *num = 0;
    br_uint_32 s = BrFileGetLength(filename);
    br_uint_32 ret = 1;

    if (s > 16) /* files with just a header aren't worth reading */
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
            BrMemReadBU32(&buffer) == 1  &&
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
                    case 35:
                        *num += 1;
                        BrMemSkipBytes(&buffer, 2);
                        BrMemReadString(&buffer);
                        break;

                    case 36:
                        BrMemReadString(&buffer);
                        break;

                    case 38:
                        BrMemReadString(&buffer);
                        break;

                    case 51:
                        BrMemSkipBytes(&buffer, 21);
                        break;

                    case 0:
                    case 37:
                    case 39:
                    case 41:
                    case 42:
                    case 43:
                    case 50:
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

		/* is this necessary? */
		BrMemFree(buffer_start);
		buffer_start = NULL;
		buffer = NULL;
    }

    return ret;
}

br_actor* BrActorLoad(const char* filename)
{
    br_actor *a;

    if (BrActorLoadMany(filename, &a, 1))
    {
        return a;
    }
    else
    {
        WARN("%s missing or corrupt", filename);
        return NULL;
    }
}

br_uint_32 BrActorLoadMany(const char* filename, br_actor** actors, br_uint_16 num)
{
	if (BrActorFileCount(filename, &num) > 0) { return 0; }

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
            br_actor *actor;
            br_intptr_t parent = NULL;

            while ((br_uint_32)buffer < pn)
            {
                blocktype = BrMemReadBU32(&buffer);
                blocksize = BrMemReadBU32(&buffer);

                switch (blocktype)
                {
                    case 35:
						/* actor node */
                        actor = BrActorAllocate(BrMemReadU8(&buffer), NULL);
                        actor->render_style = BrMemReadU8(&buffer);
                        actor->identifier = BrResStrDup(actor, BrMemReadString(&buffer));
                        //INFO("%s", actor->identifier);
                        if (parent != NULL)
                        {
                        	BrActorAdd((br_actor *)parent, actor);
                        }
                        break;

                    case 36:
						{
							/* FIXME: potential leak in model */
							br_model *m = BrModelFind(BrMemReadString(&buffer));
							if (m != NULL) { actor->model = m; }
							else { WARN("Model not linked"); }
							break;
						}

                    case 38:
						{
							/* FIXME: potential leak in material */
							br_material *m = BrMaterialFind(BrMemReadString(&buffer));
							if (m != NULL) { actor->material = m; }
							else { WARN("Material not linked"); }
							break;
						}
                        break;

                    case 51:
                        WARN("Skipped loading block %u (light data)", blocktype);
                        BrMemSkipBytes(&buffer, 21);
                        break;

                    case 43:
						/* transform */
						actor->t.type = BR_TRANSFORM_MATRIX34;
						actor->t.t.mat.m[0][0] = BrMemReadBFloat(&buffer);
                        actor->t.t.mat.m[0][1] = BrMemReadBFloat(&buffer);
                        actor->t.t.mat.m[0][2] = BrMemReadBFloat(&buffer);
                        actor->t.t.mat.m[1][0] = BrMemReadBFloat(&buffer);
                        actor->t.t.mat.m[1][1] = BrMemReadBFloat(&buffer);
                        actor->t.t.mat.m[1][2] = BrMemReadBFloat(&buffer);
                        actor->t.t.mat.m[2][0] = BrMemReadBFloat(&buffer);
                        actor->t.t.mat.m[2][1] = BrMemReadBFloat(&buffer);
                        actor->t.t.mat.m[2][2] = BrMemReadBFloat(&buffer);
                        actor->t.t.mat.m[3][0] = BrMemReadBFloat(&buffer);
                        actor->t.t.mat.m[3][1] = BrMemReadBFloat(&buffer);
                        actor->t.t.mat.m[3][2] = BrMemReadBFloat(&buffer);
                        break;

                    case 37:
						/* next block 35 is a child of this one flag? */
						parent = actor;
						break;

                    case 39:
                        /* unknown empty block */
                        break;

                    case 41:
                        /* sublevel start */
                        break;

                    case 42:
                        /* sublevel end */
                        parent = ((br_actor *)parent)->parent;
                        break;

                    case 50:
                        ((br_bounds *)(actor->type_data))->min.v[0] = BrMemReadBFloat(&buffer);
                        ((br_bounds *)(actor->type_data))->min.v[1] = BrMemReadBFloat(&buffer);
                        ((br_bounds *)(actor->type_data))->min.v[2] = BrMemReadBFloat(&buffer);
                        ((br_bounds *)(actor->type_data))->max.v[0] = BrMemReadBFloat(&buffer);
                        ((br_bounds *)(actor->type_data))->max.v[1] = BrMemReadBFloat(&buffer);
                        ((br_bounds *)(actor->type_data))->max.v[2] = BrMemReadBFloat(&buffer);
                        break;

                    case 0:
						while (actor->parent) { actor = actor->parent; }
						actors[i] = actor;
						parent = (br_intptr_t *)NULL;
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

br_actor* BrEnvironmentSet(br_actor* a)
{
	FIXME("(%p): stub", a);
	return NULL;
}

void BrLightEnable(br_actor* l)
{
	FIXME("(%p): stub", l);
	// http://www.opengl.org/sdk/docs/man/xhtml/glLight.xml
	// http://rr2000.toshiba-3.com/R4/BRENDER/TEBK_40.HTM
}

void BrClipPlaneEnable(br_actor* cp)
{
	/* FIXME("(%p): stub", cp); */
}

void BrClipPlaneDisable(br_actor* cp)
{
	/* FIXME("(%p): stub", cp); */
}
