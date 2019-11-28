#include "brender.h"
#include "model.h"

br_model* BrModelAllocate(const char* name, int nvertices, int nfaces)
{
	br_model* model = BrResAllocate(NULL, sizeof(br_model), BR_MEMORY_MODEL);

	model->identifier = (name ? BrResStrDup(model, name) : NULL);
	model->nvertices = nvertices;
	model->vertices = BrResAllocate(model, nvertices * sizeof(br_vertex), BR_MEMORY_VERTICES);
	model->nfaces = nfaces;
	model->faces = BrResAllocate(model, nfaces * sizeof(br_face), BR_MEMORY_FACES);
	model->flags = BR_MODF_UPDATEABLE;

	//model->prepared = BrResAllocate(model, sizeof(br_model_prep), BR_MEMORY_PREPARED_MODEL);
	//model->prepared->ngroups = 0;

	return model;
}

br_model* BrDefaultModel()
{
	br_scalar s1 = 1;
	br_scalar sU = 0;
	br_scalar sV = 0.99;

	br_model *model = BrModelAllocate("default_model", 8, 12);

	BrVector3Set(&model->vertices[0].p, -s1, -s1, s1);
	BrVector2Set(&model->vertices[0].map, sU, sV);

	BrVector3Set(&model->vertices[1].p, s1, -s1, s1);
	BrVector2Set(&model->vertices[1].map, sV, sV);

	BrVector3Set(&model->vertices[2].p, s1, s1, s1);
	BrVector2Set(&model->vertices[2].map, sV, sV);

	BrVector3Set(&model->vertices[3].p, -s1, s1, s1);
	BrVector2Set(&model->vertices[3].map, sU, sV);

	BrVector3Set(&model->vertices[4].p, -s1, -s1, -s1);
	BrVector2Set(&model->vertices[4].map, sU, sU);

	BrVector3Set(&model->vertices[5].p, s1, -s1, -s1);
	BrVector2Set(&model->vertices[5].map, sV, sU);

	BrVector3Set(&model->vertices[6].p, s1, s1, -s1);
	BrVector2Set(&model->vertices[6].map, sV, sU);

	BrVector3Set(&model->vertices[7].p, -s1, s1, -s1);
	BrVector2Set(&model->vertices[7].map, sU, sU);

	model->faces[0].vertices[0] = 0;
	model->faces[0].vertices[1] = 1;
	model->faces[0].vertices[2] = 2;
	model->faces[0].smoothing = 4;
	model->faces[0].flags = BR_FACEF_COPLANAR_0;

	model->faces[1].vertices[0] = 0;
	model->faces[1].vertices[1] = 2;
	model->faces[1].vertices[2] = 3;
	model->faces[1].smoothing = 1;
	model->faces[1].flags = BR_FACEF_COPLANAR_0;

	model->faces[2].vertices[0] = 0;
	model->faces[2].vertices[1] = 4;
	model->faces[2].vertices[2] = 5;
	model->faces[2].smoothing = 4;
	model->faces[2].flags = BR_FACEF_COPLANAR_0;

	model->faces[3].vertices[0] = 0;
	model->faces[3].vertices[1] = 5;
	model->faces[3].vertices[2] = 1;
	model->faces[3].smoothing = 1;
	model->faces[3].flags = BR_FACEF_COPLANAR_0;

	model->faces[4].vertices[0] = 1;
	model->faces[4].vertices[1] = 5;
	model->faces[4].vertices[2] = 6;
	model->faces[4].smoothing = 4;
	model->faces[4].flags = BR_FACEF_COPLANAR_0;

	model->faces[5].vertices[0] = 1;
	model->faces[5].vertices[1] = 6;
	model->faces[5].vertices[2] = 2;
	model->faces[5].smoothing = 1;
	model->faces[5].flags = BR_FACEF_COPLANAR_0;

	model->faces[6].vertices[0] = 2;
	model->faces[6].vertices[1] = 6;
	model->faces[6].vertices[2] = 7;
	model->faces[6].smoothing = 4;
	model->faces[6].flags = BR_FACEF_COPLANAR_0;

	model->faces[7].vertices[0] = 2;
	model->faces[7].vertices[1] = 7;
	model->faces[7].vertices[2] = 3;
	model->faces[7].smoothing = 1;
	model->faces[7].flags = BR_FACEF_COPLANAR_0;

	model->faces[8].vertices[0] = 3;
	model->faces[8].vertices[1] = 7;
	model->faces[8].vertices[2] = 4;
	model->faces[8].smoothing = 4;
	model->faces[8].flags = BR_FACEF_COPLANAR_0;

	model->faces[9].vertices[0] = 3;
	model->faces[9].vertices[1] = 4;
	model->faces[9].vertices[2] = 0;
	model->faces[9].smoothing = 1;
	model->faces[9].flags = BR_FACEF_COPLANAR_0;

	model->faces[10].vertices[0] = 4;
	model->faces[10].vertices[1] = 7;
	model->faces[10].vertices[2] = 6;
	model->faces[10].smoothing = 4;
	model->faces[10].flags = BR_FACEF_COPLANAR_0;

	model->faces[11].vertices[0] = 4;
	model->faces[11].vertices[1] = 6;
	model->faces[11].vertices[2] = 5;
	model->faces[11].smoothing = 1;
	model->faces[11].flags = BR_FACEF_COPLANAR_0;

	return model;
}

void BrModelUpdate(br_model* model, br_uint_16 flags)
{
	//if (flags != BR_MODU_ALL)
	//{
	//	INFO("%s (%uv %uf) :: %u", model->identifier, model->nvertices, model->nfaces, flags);
	//}
	INFO("%s", model->identifier);

	if (model->nvertices != 0)
	{
		br_scalar vv = 0.0;

		for (br_uint_32 i = 0; i < model->nvertices; i++)
		{
			br_scalar v = BR_MAC3(
				model->vertices[i].p.v[0], model->vertices[i].p.v[0],
				model->vertices[i].p.v[1], model->vertices[i].p.v[1],
				model->vertices[i].p.v[2], model->vertices[i].p.v[2]
			);

			if (v > vv) { vv = v; }
		}

		model->radius = sqrt(vv);

		BrVector3Copy(&model->bounds.min, &model->vertices[0].p);
		BrVector3Copy(&model->bounds.max, &model->vertices[0].p);

		if (model->nvertices > 1) {
			for (br_uint_32 i = 1; i < model->nvertices; i++) {
				model->bounds.min.v[0] = BR_MIN(model->vertices[i].p.v[0], model->bounds.min.v[0]);
				model->bounds.min.v[1] = BR_MIN(model->vertices[i].p.v[1], model->bounds.min.v[1]);
				model->bounds.min.v[2] = BR_MIN(model->vertices[i].p.v[2], model->bounds.min.v[2]);

				model->bounds.max.v[0] = BR_MAX(model->vertices[i].p.v[0], model->bounds.max.v[0]);
				model->bounds.max.v[1] = BR_MAX(model->vertices[i].p.v[1], model->bounds.max.v[1]);
				model->bounds.max.v[2] = BR_MAX(model->vertices[i].p.v[2], model->bounds.max.v[2]);
			}
		}
	}

	if (model->prepared) {
		
	}
	else {
		if (model->faces) {
			if (model->vertices) {
				if (model->nfaces > 0) {
					br_vector4 plane;

					for (br_uint_32 i = 0; i < model->nfaces; i++) {
						BrIntCalculatePlaneEquation(&plane,
							model->vertices[model->faces[i].vertices[0]].p.v,
							model->vertices[model->faces[i].vertices[1]].p.v,
							model->vertices[model->faces[i].vertices[2]].p.v
						);

						model->faces[i].n.v[0] = (plane.v[0] > -1.0 ? (plane.v[0] < 1.0 ? plane.v[0] : 1.0) : -0.9999999);
						model->faces[i].n.v[1] = (plane.v[1] > -1.0 ? (plane.v[1] < 1.0 ? plane.v[1] : 1.0) : -0.9999999);
						model->faces[i].n.v[2] = (plane.v[2] > -1.0 ? (plane.v[2] < 1.0 ? plane.v[2] : 1.0) : -0.9999999);
						model->faces[i].d = plane.v[3];
					}
				}

				model->flags &= BR_MODF_ALL;

				if (model->nfaces > 0) {
					for (br_uint_32 i = 0; i < model->nfaces; i++) {
						model->flags |= BR_MODF_ERROR;

						for (br_uint_32 j = 0; j < 3; j++) {
							if (model->faces[i].vertices[j] > model->nvertices) {
								BR_ERROR("face references invalid vertex f=%d v=%d", i, j);
							}
						}
					}
				}

				BrIntSomethingSomething(model);

				if (model->prepared) {

				}

				if (!(model->flags & BR_MODF_UPDATEABLE)) {
					if (model->faces) { BrResFree(model->faces); }
					if (model->vertices) { BrResFree(model->vertices); }
					model->nfaces = 0;
					model->nvertices = 0;
					model->faces = 0;
					model->vertices = 0;
				}
			}
		}
	}

	if (1)
	{
		/* this is becoming too wonky */
		/*
		TODO:
		* Loop through model looking for unique materials: ngroups
		* Create group for each material and assign all faces, vertices
		* Possibly calculate face edges, face normals and such
		*/

		if (!model->prepared) {
			model->prepared = BrResAllocate(model, sizeof(br_model_prep), BR_MEMORY_PREPARED_MODEL);
			model->prepared->ngroups = 0;
		}

		// Clear all previously prepared data
		for (br_uint_16 i = 0; i < model->prepared->ngroups; i++)
		{
			BrMemFree(model->prepared->groups[i].vertices);
			BrMemFree(model->prepared->groups[i].vertex_colours);
			BrMemFree(model->prepared->groups[i].vertex_user);

			BrMemFree(model->prepared->groups[i].faces);
			BrMemFree(model->prepared->groups[i].face_colours);
			BrMemFree(model->prepared->groups[i].face_user);
		}
		if (model->prepared->ngroups > 0) { BrMemFree(model->prepared->groups); }

		//// recalculate
		//ArrayList *matlist = ArrayList_createInt32();
		//ArrayList *facecount = ArrayList_createInt32();
		//ArrayList *vertcount = ArrayList_createInt32();
		//ArrayList *vertminmax = ArrayList_createInt32();
		//ArrayList *verts = ArrayList_createInt32();
		//ArrayList *edges = ArrayList_createInt32();
		//br_uint_16 *glookup = BrResAllocate(model->prepared, model->nfaces * sizeof(br_uint_16), BR_MEMORY_SCRATCH);

		//for (br_uint_32 i = 0; i < model->nfaces; i++)
		//{
		//	br_uint_32 key = (model->faces[i].material != NULL ? model->faces[i].material : 0);
		//	br_uint_32 idx = ArrayList_find(matlist, key);

		//	if (idx == -1)
		//	{
		//		idx = ArrayList_addInt32(matlist, key) - 1;
		//		ArrayList_addInt32(facecount, 0);
		//		ArrayList_addInt32(vertcount, 0);
		//		ArrayList_addInt32(vertminmax, 0xFFFF0000);
		//	}

		//	ArrayList_setInt32(facecount, idx, ArrayList_getInt32(facecount, idx) + 1);
		//	glookup[i] = idx;

		//	for (br_uint_32 j = 0; j < 3; j++)
		//	{
		//		br_uint_32 vert = (idx << 16) + model->faces[i].vertices[j];

		//		br_uint_32 min = ArrayList_getInt32(vertminmax, idx);
		//		br_uint_32 max = (min & 0xFFFF);
		//		min >>= 16;

		//		if (model->faces[i].vertices[j] < min) { min = model->faces[i].vertices[j]; }
		//		if (model->faces[i].vertices[j] > max) { max = model->faces[i].vertices[j]; }

		//		ArrayList_setInt32(vertminmax, idx, (min << 16) + max);
		//		ArrayList_setInt32(vertcount, idx, ArrayList_getInt32(vertcount, idx) + 1);
		//	}
		//}

		// errol 2018-04-18
		model->prepared->ngroups = 1;
		model->prepared->groups = BrResAllocate(model->prepared, model->prepared->ngroups * sizeof(br_group), BR_MEMORY_GROUPS);

		model->prepared->groups[0].nvertices = model->nvertices;
		model->prepared->groups[0].vertices = BrResAllocate(model->prepared, model->prepared->groups[0].nvertices * sizeof(br_vertex_prep), BR_MEMORY_PREPARED_VERTICES);
		model->prepared->groups[0].vertex_user = BrResAllocate(model->prepared, model->prepared->groups[0].nvertices * sizeof(br_uint_16), BR_MEMORY_MATERIAL_INDEX);

		for (br_uint_32 i = 0; i < model->prepared->groups[0].nvertices; i++)
		{
			BrVector3Copy(&model->prepared->groups[0].vertices[i].p, &model->vertices[i].p);
		}

		model->prepared->groups[0].nfaces = model->nfaces;
		model->prepared->groups[0].faces = BrResAllocate(model->prepared, model->prepared->groups[0].nfaces * sizeof(br_face_prep), BR_MEMORY_PREPARED_FACES);
		model->prepared->groups[0].face_colours = BrResAllocate(model->prepared, model->prepared->groups[0].nfaces * sizeof(br_colour), BR_MEMORY_MATERIAL_INDEX);
		model->prepared->groups[0].face_user = BrResAllocate(model->prepared, model->prepared->groups[0].nfaces * sizeof(br_uint_16), BR_MEMORY_MATERIAL_INDEX);

		for (br_uint_32 i = 0; i < model->prepared->groups[0].nfaces; i++)
		{
			model->prepared->groups[0].faces[i].vertices[0] = model->faces[i].vertices[0];
			model->prepared->groups[0].faces[i].vertices[1] = model->faces[i].vertices[1];
			model->prepared->groups[0].faces[i].vertices[2] = model->faces[i].vertices[2];
		}
		// errol 2018-04-18

		//model->prepared->ngroups = ArrayList_size(matlist);
		//model->prepared->groups = BrResAllocate(model->prepared, model->prepared->ngroups * sizeof(br_group), BR_MEMORY_GROUPS);

		//for (br_uint_32 i = 0; i < model->prepared->ngroups; i++)
		//{
		//	INFO("nvertices: %u", ArrayList_getInt32(vertcount, i));

		//	model->prepared->groups[i].nvertices = ArrayList_getInt32(vertcount, i);
		//	model->prepared->groups[i].vertices = BrResAllocate(model->prepared, model->prepared->groups[i].nvertices * sizeof(br_vertex_prep), BR_MEMORY_PREPARED_VERTICES);
		//	model->prepared->groups[i].vertex_colours = BrResAllocate(model->prepared, model->prepared->groups[i].nvertices * sizeof(br_colour), BR_MEMORY_MATERIAL_INDEX);
		//	model->prepared->groups[i].vertex_user = BrResAllocate(model->prepared, model->prepared->groups[i].nvertices * sizeof(br_uint_16), BR_MEMORY_MATERIAL_INDEX);

		//	model->prepared->groups[i].nfaces = ArrayList_getInt32(facecount, i);
		//	model->prepared->groups[i].faces = BrResAllocate(model->prepared, model->prepared->groups[i].nfaces * sizeof(br_face_prep), BR_MEMORY_PREPARED_FACES);
		//	model->prepared->groups[i].face_colours = BrResAllocate(model->prepared, model->prepared->groups[i].nfaces * sizeof(br_colour), BR_MEMORY_MATERIAL_INDEX);
		//	model->prepared->groups[i].face_user = BrResAllocate(model->prepared, model->prepared->groups[i].nfaces * sizeof(br_uint_16), BR_MEMORY_MATERIAL_INDEX);

		//	ArrayList_clear(verts);
		//	ArrayList_clear(edges);

		//	br_uint_32 face = 0;
		//	br_uint_32 min = ArrayList_getInt32(vertminmax, i);
		//	br_uint_32 max = (min & 0xFFFF);
		//	min >>= 16;

		//	for (br_uint_32 j = 0; j < model->nfaces; j++)
		//	{
		//		if (glookup[j] != i) { continue; }

		//		for (br_uint_32 k = 0; k < 3; k++)
		//		{
		//			br_uint_32 vert = model->faces[j].vertices[k];
		//			br_uint_32 vidx = ArrayList_find(verts, vert);

		//			if (vidx == -1)
		//			{
		//				br_uint_32 vcount = ArrayList_size(verts);

		//				INFO("%u - %u < %u", vert, min, model->prepared->groups[i].nvertices);

		//				model->prepared->groups[i].vertex_user[vcount] = (j * 3) + k;
		//				BrVector3Copy(&model->prepared->groups[i].vertices[vert - min].p, &model->vertices[vert].p);
		//				BrVector2Copy(&model->prepared->groups[i].vertices[vert - min].map, &model->vertices[vert].map);

		//				vidx = ArrayList_addInt32(verts, vert) - 1;
		//			}

		//			model->prepared->groups[i].faces[face].vertices[k] = vert - min;
		//		}

		//		for (br_uint_32 k = 0; k < 3; k++)
		//		{
		//			br_uint_32 e1 = model->prepared->groups[i].faces[face].vertices[k];
		//			br_uint_32 e2 = model->prepared->groups[i].faces[face].vertices[(k < 2 ? k + 1 : 0)];

		//			if (e2 < e1) { e1 = e2; e2 = model->prepared->groups[i].faces[face].vertices[k]; }

		//			br_uint_32 edge = (e1 << 16) + e2;
		//			br_uint_32 eidx = ArrayList_find(edges, edge);

		//			if (debug)
		//			{
		//				INFO("%u %u :: %u == %u", i, e1, e2, eidx);
		//			}

		//			if (eidx == -1) { eidx = ArrayList_addInt32(edges, edge) - 1; }

		//			model->prepared->groups[i].faces[face].edges[k] = eidx;
		//		}

		//		static br_vector3 v1, v2, v3;

		//		BrVector3Copy(&v1, &model->prepared->groups[i].vertices[model->prepared->groups[i].faces[face].vertices[0]].p);
		//		BrVector3Copy(&v2, &model->prepared->groups[i].vertices[model->prepared->groups[i].faces[face].vertices[1]].p);
		//		BrVector3Copy(&v3, &model->prepared->groups[i].vertices[model->prepared->groups[i].faces[face].vertices[2]].p);

		//		static br_vector3 v12, v13, n;
		//		BrVector3Sub(&v12, &v2, &v1);
		//		BrVector3Sub(&v13, &v3, &v1);
		//		BrVector3Cross(&n, &v12, &v13);
		//		BrVector3Normalise(&n, &n);

		//		model->prepared->groups[i].faces[face].eqn.v[0] = n.v[0];
		//		model->prepared->groups[i].faces[face].eqn.v[1] = n.v[1];
		//		model->prepared->groups[i].faces[face].eqn.v[2] = n.v[2];
		//		model->prepared->groups[i].faces[face].eqn.v[3] = BrVector3Dot(&n, &v1);

		//		face++;
		//	}

		//	model->prepared->groups[i].nedges = ArrayList_size(edges) + 1;
		//}

		//model->prepared->size = sizeof(model->prepared);

		//ArrayList_free(matlist);
		//ArrayList_free(facecount);
		//ArrayList_free(vertcount);
		//ArrayList_free(vertminmax);
		//ArrayList_free(verts);
		//ArrayList_free(edges);
		//BrMemFree(glookup);
	}
}

br_uint_32 BrModelFileCount(const char* filename, br_uint_16* num)
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
			BrMemReadBU32(&buffer) == 8 &&
			BrMemReadBU32(&buffer) == 64206 &&
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
				case 54:
					*num += 1;
					BrMemSkipBytes(&buffer, 2);
					BrMemReadString(&buffer);
					break;

				case 22:
				{
					br_uint_32 j = BrMemReadBU32(&buffer);
					for (int k = 0; k < j; k++)
					{
						BrMemReadString(&buffer);
					}
					break;
				}

				case 0:
				case 23:
				case 24:
				case 26:
				case 53:
					BrMemSkipBytes(&buffer, blocksize);
					break;

				default:
					INFO("%s:", filename);
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

br_model* BrModelLoad(const char* filename)
{
	br_model *m;

	if (BrModelLoadMany(filename, &m, 1))
	{
		return m;
	}
	else
	{
		WARN("%s missing or corrupt", filename);
		return NULL;
	}
}

br_uint_32 BrModelLoadMany(const char* filename, br_model** models, br_uint_16 num)
{
	if (BrModelFileCount(filename, &num) > 0) { return 0; }

	if (num > 0)
	{
		br_uint_32 s = BrFileGetLength(filename);

		if (s)
		{
			INFO("%s", filename);
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

			br_intptr_t *materials;

			while ((br_uint_32)buffer < pn)
			{
				blocktype = BrMemReadBU32(&buffer);
				blocksize = BrMemReadBU32(&buffer);

				switch (blocktype)
				{
				case 54:
					BrMemSkipBytes(&buffer, 2);
					models[i] = BrModelAllocate(BrMemReadString(&buffer), 0, 0);
					break;

				case 23:
				{
					br_uint_32 nvert = BrMemReadBU32(&buffer);

					models[i]->nvertices = nvert;
					models[i]->vertices = BrResAllocate(models[i], nvert * sizeof(br_vertex), BR_MEMORY_VERTICES);

					br_float x, y, z;

					for (int j = 0; j < nvert; j++)
					{
						x = BrMemReadBFloat(&buffer);
						y = BrMemReadBFloat(&buffer);
						z = BrMemReadBFloat(&buffer);

						BrVector3SetFloat(&models[i]->vertices[j].p, x, y, z);
					}
					break;
				}

				case 24:
				{
					br_uint_32 nuv = BrMemReadBU32(&buffer);

					br_float u, v;

					for (int j = 0; j < nuv; j++)
					{
						u = BrMemReadBFloat(&buffer);
						v = BrMemReadBFloat(&buffer);

						BrVector2SetFloat(&models[i]->vertices[j].map, u, v);
					}
					break;
				}

				case 53:
				{
					br_uint_32 nface = BrMemReadBU32(&buffer);

					models[i]->nfaces = nface;
					models[i]->faces = BrResAllocate(models[i], nface * sizeof(br_face), BR_MEMORY_FACES);

					for (int j = 0; j < nface; j++)
					{
						models[i]->faces[j].vertices[0] = BrMemReadBU16(&buffer);
						models[i]->faces[j].vertices[1] = BrMemReadBU16(&buffer);
						models[i]->faces[j].vertices[2] = BrMemReadBU16(&buffer);
						models[i]->faces[j].smoothing = BrMemReadBU16(&buffer);
						models[i]->faces[j].flags = BrMemReadU8(&buffer);
					}
					break;
				}

				case 22:
				{
					br_uint_32 nmaterials = BrMemReadBU32(&buffer);
					materials = BrMemAllocate(nmaterials * sizeof(br_intptr_t), BR_MEMORY_MATERIAL_INDEX);

					for (int j = 0; j < nmaterials; j++)
					{
						materials[j] = (br_intptr_t)BrMaterialFind(BrMemReadString(&buffer));
						if (materials[j] == NULL) { materials[j] = DEFAULT_MATERIAL; }
					}

					break;
				}

				case 26:
				{
					br_uint_32 nfacemats = BrMemReadBU32(&buffer);
					BrMemReadBU32(&buffer); /* unknown */

					for (int j = 0; j < nfacemats; j++)
					{
						br_uint_16 matindex = BrMemReadBU16(&buffer);
						models[i]->faces[j].material = (matindex == 0 ? DEFAULT_MATERIAL : (br_material *)materials[matindex - 1]);
					}

					if (materials != NULL) { BrMemFree(materials); }
					break;
				}

				case 0:
					i++;
					break;

				default:
					INFO("%s:", filename);
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

br_uint_32 BrModelSaveMany(const char* filename, const br_model* const* models, br_uint_16 num)
{
	FIXME("(\"%s\", %p, %d): stub", filename, models, num);
	return 0;
}

br_model* BrModelAdd(br_model* model)
{
	BrListInsert(reg_models, model);
	BrModelUpdate(model, BR_MODU_ALL);
	return model;
}

br_uint_32 BrModelAddMany(br_model* const* models, int n)
{
	br_uint_32 added = 0;

	for (int i = 0; i < n; i++)
		if (BrModelAdd(models[i]))
			added++;

	return added;
}

br_model* BrModelFind(const char* pattern)
{
	return (br_model *)BrListFind(reg_models, pattern);
}

br_model* BrModelRemove(br_model* model)
{
	return (br_model *)BrListRemove(reg_models, model);
}

void BrModelFree(br_model* m)
{
	INFO("%p->%s", m, m->identifier);
	BrMemFree(m->identifier);
	BrMemFree(m->vertices);
	BrMemFree(m->faces);

	if (m->prepared) {
		for (br_uint_8 i = 0; i < m->prepared->ngroups; i++)
		{
			BrMemFree(m->prepared->groups[i].vertices);
			BrMemFree(m->prepared->groups[i].vertex_colours);
			BrMemFree(m->prepared->groups[i].vertex_user);
			BrMemFree(m->prepared->groups[i].faces);
			BrMemFree(m->prepared->groups[i].face_colours);
			BrMemFree(m->prepared->groups[i].face_user);
		}
		BrMemFree(m->prepared->groups);
		BrMemFree(m->prepared);
	}

	BrMemFree(m);
}

void BrIntCalculatePlaneEquation(br_vector4* plane, br_vector3* v1, br_vector3* v2, br_vector3* v3)
{
	br_vector3 v12, v13, v;
	
	BrVector3Sub(&v12, v2, v1);
	BrVector3Sub(&v13, v3, v1);
	BrVector3Cross(&v, &v12, &v13);
	BrVector3Normalise(&v, &v);

	plane->v[0] = v.v[0];
	plane->v[1] = v.v[1];
	plane->v[2] = v.v[2];
	plane->v[3] = BrVector3Dot(&v, v1);
}

void BrIntSort(void *base, size_t nitems, size_t size, int(*compar)(const void *, const void*)) {
	qsort(base, nitems, size, compar);
}

void BrIntSomethingSomething(br_model* model) {
	int sort;
	br_uint_32 n1, n2;
	void* scratch;

	/*
	004D83CF  |.  C74424 1C 708 MOV DWORD PTR SS:[LOCAL.8],m004D8A70     ; Entry point
	*/
	sort = (model->flags & BR_MODF_DONT_WELD ? m004D8AF0 : m004D8C50);

	if (model->flags & BR_MODF_UNKNOWN) {
		/*
		004D83ED  |>  F6C4 01       TEST AH,01
		004D83F0  |.  74 28         JZ SHORT 004D841A
		004D83F2  |.  33C0          XOR EAX,EAX
		004D83F4  |.  8B4C24 4C     MOV ECX,DWORD PTR SS:[ARG.1]
		004D83F8  |.  C74424 2C B08 MOV DWORD PTR SS:[LOCAL.6],m004D8DB0      ; Entry point
		004D8400  |.  66:8B41 2C    MOV AX,WORD PTR DS:[ECX+2C]
		004D8404  |.  894424 10     MOV DWORD PTR SS:[LOCAL.13],EAX
		004D8408  |.  DB4424 10     FILD DWORD PTR SS:[LOCAL.13]
		004D840C  |.  DC0D E0595000 FMUL QWORD PTR DS:[5059E0]               ; FLOAT 9.587379924285257e-05
		004D8412  |.  D9FF          FCOS
		004D8414  |.  D95C24 14     FSTP DWORD PTR SS:[LOCAL.12]
		*/
	}
	else {
		/*
		004D841A  |>  C74424 14 000 MOV DWORD PTR SS:[LOCAL.12],0
		004D8422  |.  C74424 2C 008 MOV DWORD PTR SS:[LOCAL.6],m004D8D00      ; Entry point
		*/
	}
	/*
	004D842A  |>  8B4C24 4C     MOV ECX,DWORD PTR SS:[ARG.1]
	004D842E  |.  33D2          XOR EDX,EDX
	004D8430  |.  890D 44165400 MOV DWORD PTR DS:[541644],ECX
	*/
	n1 = model->nfaces * 2 + model->nfaces;
	n2 = n1 + n1 * 4 + model->nfaces;
	scratch = BrIntAllocateScratchpad(n2 * 4);
	BrIntMemSet(scratch, 0, n2);

	// scratch + (n1 << 4)

	/*
	004D8463  |.  8B4424 24     MOV EAX,n1
	004D8467  |.  8B4C24 24     MOV ECX,n1
	004D846B  |.  C1E0 04       SHL EAX,4
	004D846E  |.  8B7C24 48     MOV EDI,scratch
	004D8472  |.  034424 48     ADD EAX,scratch

	004D8476  |.  83C4 0C       ADD ESP,0C
	004D8479  |.  8D1488        LEA EDX,[ECX*4+EAX]
	004D847C  |.  894424 28     MOV DWORD PTR SS:[LOCAL.7],EAX
	004D8480  |.  8B4C24 4C     MOV ECX,DWORD PTR SS:[ARG.1]
	004D8484  |.  895424 1C     MOV DWORD PTR SS:[LOCAL.10],EDX
	004D8488  |.  33D2          XOR EDX,EDX
	004D848A  |.  66:3951 12    CMP WORD PTR DS:[ECX+12],DX
	004D848E  |.  8B59 0C       MOV EBX,DWORD PTR DS:[ECX+0C]
	004D8491  |.  76 4E         JBE SHORT 004D84E1
	004D8493  |.  8BF0          MOV ESI,EAX
	004D8495  |.  8B4424 1C     MOV EAX,DWORD PTR SS:[LOCAL.10]
	004D8499  |.  894424 10     MOV DWORD PTR SS:[LOCAL.13],EAX
	004D849D  |>  8B4424 10     /MOV EAX,DWORD PTR SS:[LOCAL.13]
	004D84A1  |.  8BCB          |MOV ECX,EBX
	004D84A3  |.  8918          |MOV DWORD PTR DS:[EAX],EBX
	004D84A5  |.  B8 03000000   |MOV EAX,3
	004D84AA  |>  66:8B29       |/MOV BP,WORD PTR DS:[ECX]
	004D84AD  |.  83C1 02       ||ADD ECX,2
	004D84B0  |.  66:896F 0C    ||MOV WORD PTR DS:[EDI+0C],BP
	004D84B4  |.  83C6 04       ||ADD ESI,4
	004D84B7  |.  66:8957 0E    ||MOV WORD PTR DS:[EDI+0E],DX
	004D84BB  |.  897E FC       ||MOV DWORD PTR DS:[ESI-4],EDI
	004D84BE  |.  83C7 10       ||ADD EDI,10
	004D84C1  |.  48            ||DEC EAX
	004D84C2  |.^ 75 E6         |\JNZ SHORT 004D84AA
	004D84C4  |.  8B4424 10     |MOV EAX,DWORD PTR SS:[LOCAL.13]
	004D84C8  |.  42            |INC EDX
	004D84C9  |.  83C0 04       |ADD EAX,4
	004D84CC  |.  83C3 28       |ADD EBX,28
	004D84CF  |.  8B4C24 4C     |MOV ECX,DWORD PTR SS:[ARG.1]
	004D84D3  |.  894424 10     |MOV DWORD PTR SS:[LOCAL.13],EAX
	004D84D7  |.  33C0          |XOR EAX,EAX
	004D84D9  |.  66:8B41 12    |MOV AX,WORD PTR DS:[ECX+12]
	004D84DD  |.  3BC2          |CMP EAX,EDX
	004D84DF  |.^ 7F BC         \JG SHORT 004D849D
	004D84E1  |>  68 408A4D00   PUSH 004D8A40                            ; /Arg4 = CARMA95.4D8A40, Entry point
	004D84E6  |.  33C0          XOR EAX,EAX                              ; |
	004D84E8  |.  8B4C24 50     MOV ECX,DWORD PTR SS:[ARG.1]             ; |
	004D84EC  |.  6A 04         PUSH 4                                   ; |Arg3 = 4
	004D84EE  |.  66:8B41 12    MOV AX,WORD PTR DS:[ECX+12]              ; |
	004D84F2  |.  8B5424 24     MOV EDX,DWORD PTR SS:[LOCAL.10]          ; |
	004D84F6  |.  50            PUSH EAX                                 ; |Arg2
	004D84F7  |.  52            PUSH EDX                                 ; |Arg1 => [LOCAL.10]
	004D84F8  |.  E8 83DA0000   CALL BrIntSort                           ; \CARMA95.BrIntSort
	004D84FD  |.  8B6C24 28     MOV EBP,n1
	004D8501  |.  8B4C24 34     MOV ECX,DWORD PTR SS:[LOCAL.8]
	004D8505  |.  8B4424 38     MOV EAX,DWORD PTR SS:[LOCAL.7]
	004D8509  |.  83C4 10       ADD ESP,10
	004D850C  |.  33DB          XOR EBX,EBX
	004D850E  |.  33F6          XOR ESI,ESI
	004D8510  |.  51            PUSH ECX                                 ; /Arg4 => [LOCAL.8]
	004D8511  |.  6A 04         PUSH 4                                   ; |Arg3 = 4
	004D8513  |.  55            PUSH EBP                                 ; |Arg2 => n1
	004D8514  |.  50            PUSH EAX                                 ; |Arg1 => [LOCAL.7]
	004D8515  |.  E8 66DA0000   CALL BrIntSort                           ; \CARMA95.BrIntSort
	004D851A  |.  8B4C24 28     MOV ECX,n1
	004D851E  |.  83C4 10       ADD ESP,10
	004D8521  |.  49            DEC ECX
	004D8522  |.  85C9          TEST ECX,ECX
	004D8524  |.  894C24 30     MOV DWORD PTR SS:[LOCAL.5],ECX
	004D8528  |.  7E 3C         JLE SHORT 004D8566
	004D852A  |.  8B7C24 28     MOV EDI,DWORD PTR SS:[LOCAL.7]
	004D852E  |.  83C7 04       ADD EDI,4
	004D8531  |>  57            /PUSH EDI
	004D8532  |.  8D47 FC       |LEA EAX,[EDI-4]
	004D8535  |.  50            |PUSH EAX
	004D8536  |.  FF5424 2C     |CALL DWORD PTR SS:[LOCAL.8]
	004D853A  |.  83C4 08       |ADD ESP,8
	004D853D  |.  85C0          |TEST EAX,EAX
	004D853F  |.  74 1B         |JZ SHORT 004D855C
	004D8541  |.  8B4424 28     |MOV EAX,DWORD PTR SS:[ESP+28]
	004D8545  |.  57            |PUSH EDI
	004D8546  |.  8B5424 18     |MOV EDX,DWORD PTR SS:[ESP+18]
	004D854A  |.  90            |NOP
	004D854B  |.  8D0CB0        |LEA ECX,[ESI*4+EAX]
	004D854E  |.  8B4424 50     |MOV EAX,DWORD PTR SS:[ESP+50]
	004D8552  |.  51            |PUSH ECX
	004D8553  |.  8D73 01       |LEA ESI,[EBX+1]
	004D8556  |.  52            |PUSH EDX
	004D8557  |.  50            |PUSH EAX
	004D8558  |.  FF5424 3C     |CALL DWORD PTR SS:[ESP+3C]
	004D855C  |>  83C7 04       |ADD EDI,4
	004D855F  |.  43            |INC EBX
	004D8560  |.  3B5C24 30     |CMP EBX,DWORD PTR SS:[ESP+30]
	004D8564  |.^ 7C CB         \JL SHORT 004D8531
	004D8566  |>  8B4C24 1C     MOV ECX,DWORD PTR SS:[ESP+1C]
	004D856A  |.  8B4424 28     MOV EAX,DWORD PTR SS:[ESP+28]
	004D856E  |.  51            PUSH ECX
	004D856F  |.  8B4C24 18     MOV ECX,DWORD PTR SS:[ESP+18]
	004D8573  |.  8D14B0        LEA EDX,[ESI*4+EAX]
	004D8576  |.  8B4424 50     MOV EAX,DWORD PTR SS:[ESP+50]
	004D857A  |.  52            PUSH EDX
	004D857B  |.  51            PUSH ECX
	004D857C  |.  50            PUSH EAX
	004D857D  |.  FF5424 3C     CALL DWORD PTR SS:[ESP+3C]
	004D8581  |.  8B4C24 40     MOV ECX,DWORD PTR SS:[ESP+40]
	004D8585  |.  8B5424 28     MOV EDX,DWORD PTR SS:[ESP+28]
	004D8589  |.  51            PUSH ECX                                 ; /Arg4
	004D858A  |.  BB 01000000   MOV EBX,1                                ; |
	004D858F  |.  6A 04         PUSH 4                                   ; |Arg3 = 4
	004D8591  |.  55            PUSH EBP                                 ; |Arg2 => n1
	004D8592  |.  52            PUSH EDX                                 ; |Arg1
	004D8593  |.  E8 E8D90000   CALL BrIntSort                           ; \CARMA95.BrIntSort
	004D8598  |.  8B4424 40     MOV EAX,DWORD PTR SS:[ESP+40]
	004D859C  |.  83C4 10       ADD ESP,10
	004D859F  |.  85C0          TEST EAX,EAX
	004D85A1  |.  7E 20         JLE SHORT 004D85C3
	004D85A3  |.  8B7424 28     MOV ESI,DWORD PTR SS:[ESP+28]
	004D85A7  |.  8BF8          MOV EDI,EAX
	004D85A9  |.  83C6 04       ADD ESI,4
	004D85AC  |>  56            /PUSH ESI
	004D85AD  |.  8D46 FC       |LEA EAX,[ESI-4]
	004D85B0  |.  50            |PUSH EAX
	004D85B1  |.  FF5424 48     |CALL DWORD PTR SS:[ESP+48]
	004D85B5  |.  83C4 08       |ADD ESP,8
	004D85B8  |.  85C0          |TEST EAX,EAX
	004D85BA  |.  74 01         |JZ SHORT 004D85BD
	004D85BC  |.  43            |INC EBX
	004D85BD  |>  83C6 04       |ADD ESI,4
	004D85C0  |.  4F            |DEC EDI
	004D85C1  |.^ 75 E9         \JNZ SHORT 004D85AC
	004D85C3  |>  C74424 38 010 MOV DWORD PTR SS:[ESP+38],1
	004D85CB  |.  8B4C24 4C     MOV ECX,DWORD PTR SS:[ESP+4C]
	004D85CF  |.  66:8B49 12    MOV CX,WORD PTR DS:[ECX+12]
	004D85D3  |.  66:83F9 01    CMP CX,1
	004D85D7  |.  76 26         JBE SHORT 004D85FF
	004D85D9  |.  8B5424 1C     MOV EDX,DWORD PTR SS:[ESP+1C]
	004D85DD  |.  8BF1          MOV ESI,ECX
	004D85DF  |.  81E6 FFFF0000 AND ESI,0000FFFF
	004D85E5  |.  4E            DEC ESI
	004D85E6  |>  8B7A 04       /MOV EDI,DWORD PTR DS:[EDX+4]
	004D85E9  |.  8B02          |MOV EAX,DWORD PTR DS:[EDX]
	004D85EB  |.  8B68 08       |MOV EBP,DWORD PTR DS:[EAX+8]
	004D85EE  |.  8B47 08       |MOV EAX,DWORD PTR DS:[EDI+8]
	004D85F1  |.  3BC5          |CMP EAX,EBP
	004D85F3  |.  74 04         |JE SHORT 004D85F9
	004D85F5  |.  FF4424 38     |INC DWORD PTR SS:[ESP+38]
	004D85F9  |>  83C2 04       |ADD EDX,4
	004D85FC  |.  4E            |DEC ESI
	004D85FD  |.^ 75 E7         \JNZ SHORT 004D85E6
	004D85FF  |>  81E1 FFFF0000 AND ECX,0000FFFF
	004D8605  |.  8B4424 38     MOV EAX,DWORD PTR SS:[ESP+38]
	004D8609  |.  C1E0 02       SHL EAX,2
	004D860C  |.  8D2CCD 000000 LEA EBP,[ECX*8]
	004D8613  |.  2BE9          SUB EBP,ECX
	004D8615  |.  8D04C0        LEA EAX,[EAX*8+EAX]
	004D8618  |.  C1E5 02       SHL EBP,2
	004D861B  |.  83C0 1F       ADD EAX,1F
	004D861E  |.  83C5 1F       ADD EBP,1F
	004D8621  |.  83E0 E0       AND EAX,FFFFFFE0
	004D8624  |.  83E5 E0       AND EBP,FFFFFFE0
	004D8627  |.  894424 10     MOV DWORD PTR SS:[ESP+10],EAX
	004D862B  |.  8D049D 000000 LEA EAX,[EBX*4]
	004D8632  |.  83C0 1F       ADD EAX,1F
	004D8635  |.  8BFB          MOV EDI,EBX
	004D8637  |.  C1E7 05       SHL EDI,5
	004D863A  |.  83E0 E0       AND EAX,FFFFFFE0
	004D863D  |.  83C7 1F       ADD EDI,1F
	004D8640  |.  894424 24     MOV DWORD PTR SS:[ESP+24],EAX
	004D8644  |.  8D048D 000000 LEA EAX,[ECX*4]
	004D864B  |.  83E7 E0       AND EDI,FFFFFFE0
	004D864E  |.  83C0 1F       ADD EAX,1F
	004D8651  |.  03CB          ADD ECX,EBX
	004D8653  |.  83E0 E0       AND EAX,FFFFFFE0
	004D8656  |.  8B5424 10     MOV EDX,DWORD PTR SS:[ESP+10]
	004D865A  |.  8B7424 24     MOV ESI,DWORD PTR SS:[ESP+24]
	004D865E  |.  894424 14     MOV DWORD PTR SS:[ESP+14],EAX
	004D8662  |.  8D444D 20     LEA EAX,[ECX*2+EBP+20]
	004D8666  |.  8B4C24 14     MOV ECX,DWORD PTR SS:[ESP+14]
	004D866A  |.  03C2          ADD EAX,EDX
	004D866C  |.  8B5424 4C     MOV EDX,DWORD PTR SS:[ESP+4C]
	004D8670  |.  03C6          ADD EAX,ESI
	004D8672  |.  03C1          ADD EAX,ECX
	004D8674  |.  8D3438        LEA ESI,[EDI+EAX]
	004D8677  |.  8B42 4C       MOV EAX,DWORD PTR DS:[EDX+4C]
	004D867A  |.  85C0          TEST EAX,EAX
	004D867C  |.  74 1A         JZ SHORT 004D8698
	004D867E  |.  8B08          MOV ECX,DWORD PTR DS:[EAX]
	004D8680  |.  3BCE          CMP ECX,ESI
	004D8682  |.  73 14         JAE SHORT 004D8698
	004D8684  |.  50            PUSH EAX                                 ; /Arg1
	004D8685  |.  E8 D66D0000   CALL 004DF460                            ; \CARMA95.004DF460
	004D868A  |.  8B4C24 50     MOV ECX,DWORD PTR SS:[ESP+50]
	004D868E  |.  83C4 04       ADD ESP,4
	004D8691  |.  C741 4C 00000 MOV DWORD PTR DS:[ECX+4C],0
	004D8698  |>  8B4C24 4C     MOV ECX,DWORD PTR SS:[ESP+4C]
	004D869C  |.  8B41 4C       MOV EAX,DWORD PTR DS:[ECX+4C]
	004D869F  |.  85C0          TEST EAX,EAX
	004D86A1  |.  75 15         JNZ SHORT 004D86B8
	004D86A3  |.  6A 2A         PUSH 2A                                  ; /Arg3 = 2A
	004D86A5  |.  56            PUSH ESI                                 ; |Arg2
	004D86A6  |.  51            PUSH ECX                                 ; |Arg1
	004D86A7  |.  E8 C46C0000   CALL 004DF370                            ; \CARMA95.004DF370
	004D86AC  |.  8B4C24 58     MOV ECX,DWORD PTR SS:[ESP+58]
	004D86B0  |.  83C4 0C       ADD ESP,0C
	004D86B3  |.  8941 4C       MOV DWORD PTR DS:[ECX+4C],EAX
	004D86B6  |.  8930          MOV DWORD PTR DS:[EAX],ESI
	004D86B8  |>  8B4C24 4C     MOV ECX,DWORD PTR SS:[ESP+4C]
	004D86BC  |.  8B4424 10     MOV EAX,DWORD PTR SS:[ESP+10]
	004D86C0  |.  8B71 4C       MOV ESI,DWORD PTR DS:[ECX+4C]
	004D86C3  |.  8BCE          MOV ECX,ESI
	004D86C5  |.  83C6 20       ADD ESI,20
	004D86C8  |.  897424 20     MOV DWORD PTR SS:[ESP+20],ESI
	004D86CC  |.  03F0          ADD ESI,EAX
	004D86CE  |.  8BC6          MOV EAX,ESI
	004D86D0  |.  8B5424 24     MOV EDX,DWORD PTR SS:[ESP+24]
	004D86D4  |.  03F5          ADD ESI,EBP
	004D86D6  |.  8B6C24 14     MOV EBP,DWORD PTR SS:[ESP+14]
	004D86DA  |.  897424 44     MOV DWORD PTR SS:[ESP+44],ESI
	004D86DE  |.  03F7          ADD ESI,EDI
	004D86E0  |.  897424 2C     MOV DWORD PTR SS:[ESP+2C],ESI
	004D86E4  |.  03F2          ADD ESI,EDX
	004D86E6  |.  66:8B7C24 38  MOV DI,WORD PTR SS:[ESP+38]
	004D86EB  |.  8BD6          MOV EDX,ESI
	004D86ED  |.  03F5          ADD ESI,EBP
	004D86EF  |.  897424 10     MOV DWORD PTR SS:[ESP+10],ESI
	004D86F3  |.  90            NOP
	004D86F4  |.  8D345E        LEA ESI,[EBX*2+ESI]
	004D86F7  |.  8B5C24 20     MOV EBX,DWORD PTR SS:[ESP+20]
	004D86FB  |.  8959 18       MOV DWORD PTR DS:[ECX+18],EBX
	004D86FE  |.  66:8979 08    MOV WORD PTR DS:[ECX+8],DI
	004D8702  |.  8B4C24 1C     MOV ECX,DWORD PTR SS:[ESP+1C]
	004D8706  |.  8943 04       MOV DWORD PTR DS:[EBX+4],EAX
	004D8709  |.  8953 08       MOV DWORD PTR DS:[EBX+8],EDX
	004D870C  |.  8973 0C       MOV DWORD PTR DS:[EBX+0C],ESI
	004D870F  |.  8B29          MOV EBP,DWORD PTR DS:[ECX]
	004D8711  |.  8B4D 08       MOV ECX,DWORD PTR SS:[EBP+8]
	004D8714  |.  85C9          TEST ECX,ECX
	004D8716  |.  74 09         JZ SHORT 004D8721
	004D8718  |.  8B49 70       MOV ECX,DWORD PTR DS:[ECX+70]
	004D871B  |.  8BEB          MOV EBP,EBX
	004D871D  |.  890B          MOV DWORD PTR DS:[EBX],ECX
	004D871F  |.  EB 0B         JMP SHORT 004D872C
	004D8721  |>  8B6C24 20     MOV EBP,DWORD PTR SS:[ESP+20]
	004D8725  |.  C745 00 00000 MOV DWORD PTR SS:[EBP],0
	004D872C  |>  8B6C24 20     MOV EBP,DWORD PTR SS:[ESP+20]
	004D8730  |.  33FF          XOR EDI,EDI
	004D8732  |.  C74424 34 000 MOV DWORD PTR SS:[ESP+34],0
	004D873A  |.  8D5D 1C       LEA EBX,[EBP+1C]
	004D873D  |.  8B4C24 4C     MOV ECX,DWORD PTR SS:[ESP+4C]
	004D8741  |.  C74424 18 000 MOV DWORD PTR SS:[ESP+18],0
	004D8749  |.  66:C703 0000  MOV WORD PTR DS:[EBX],0
	004D874E  |.  66:8B79 12    MOV DI,WORD PTR DS:[ECX+12]
	004D8752  |.  4F            DEC EDI
	004D8753  |.  85FF          TEST EDI,EDI
	004D8755  |.  0F8E B9000000 JLE 004D8814
	004D875B  |.  83C6 02       ADD ESI,2
	004D875E  |.  8BEB          MOV EBP,EBX
	004D8760  |.  83C2 04       ADD EDX,4
	004D8763  |.  83C0 1C       ADD EAX,1C
	004D8766  |.  8B7C24 1C     MOV EDI,DWORD PTR SS:[ESP+1C]
	004D876A  |.  895424 24     MOV DWORD PTR SS:[ESP+24],EDX
	004D876E  |.  83C7 04       ADD EDI,4
	004D8771  |.  894424 14     MOV DWORD PTR SS:[ESP+14],EAX
	004D8775  |>  8B4C24 4C     /MOV ECX,DWORD PTR SS:[ESP+4C]
	004D8779  |.  8B47 FC       |MOV EAX,DWORD PTR DS:[EDI-4]
	004D877C  |.  51            |PUSH ECX                                ; /Arg4
	004D877D  |.  50            |PUSH EAX                                ; |Arg3
	004D877E  |.  33C0          |XOR EAX,EAX                             ; |
	004D8780  |.  8D4D E4       |LEA ECX,[EBP-1C]                        ; |
	004D8783  |.  66:8B45 00    |MOV AX,WORD PTR SS:[EBP]                ; |
	004D8787  |.  50            |PUSH EAX                                ; |Arg2
	004D8788  |.  51            |PUSH ECX                                ; |Arg1
	004D8789  |.  E8 52080000   |CALL 004D8FE0                           ; \CARMA95.004D8FE0
	004D878E  |.  66:FF45 00    |INC WORD PTR SS:[EBP]
	004D8792  |.  8B0F          |MOV ECX,DWORD PTR DS:[EDI]
	004D8794  |.  8B57 FC       |MOV EDX,DWORD PTR DS:[EDI-4]
	004D8797  |.  8B42 08       |MOV EAX,DWORD PTR DS:[EDX+8]
	004D879A  |.  8B51 08       |MOV EDX,DWORD PTR DS:[ECX+8]
	004D879D  |.  3BD0          |CMP EDX,EAX
	004D879F  |.  74 3B         |JE SHORT 004D87DC
	004D87A1  |.  83C5 24       |ADD EBP,24
	004D87A4  |.  8B4424 18     |MOV EAX,DWORD PTR SS:[ESP+18]
	004D87A8  |.  40            |INC EAX
	004D87A9  |.  8B4C24 14     |MOV ECX,DWORD PTR SS:[ESP+14]
	004D87AD  |.  8B5424 24     |MOV EDX,DWORD PTR SS:[ESP+24]
	004D87B1  |.  894424 18     |MOV DWORD PTR SS:[ESP+18],EAX
	004D87B5  |.  894D E8       |MOV DWORD PTR SS:[EBP-18],ECX
	004D87B8  |.  8955 EC       |MOV DWORD PTR SS:[EBP-14],EDX
	004D87BB  |.  8975 F0       |MOV DWORD PTR SS:[EBP-10],ESI
	004D87BE  |.  8B07          |MOV EAX,DWORD PTR DS:[EDI]
	004D87C0  |.  8B40 08       |MOV EAX,DWORD PTR DS:[EAX+8]
	004D87C3  |.  85C0          |TEST EAX,EAX
	004D87C5  |.  74 08         |JZ SHORT 004D87CF
	004D87C7  |.  8B40 70       |MOV EAX,DWORD PTR DS:[EAX+70]
	004D87CA  |.  8945 E4       |MOV DWORD PTR SS:[EBP-1C],EAX
	004D87CD  |.  EB 07         |JMP SHORT 004D87D6
	004D87CF  |>  C745 E4 00000 |MOV DWORD PTR SS:[EBP-1C],0
	004D87D6  |>  66:C745 00 00 |MOV WORD PTR SS:[EBP],0
	004D87DC  |>  83C6 02       |ADD ESI,2
	004D87DF  |.  8B4424 24     |MOV EAX,DWORD PTR SS:[ESP+24]
	004D87E3  |.  83C0 04       |ADD EAX,4
	004D87E6  |.  8B4C24 14     |MOV ECX,DWORD PTR SS:[ESP+14]
	004D87EA  |.  83C1 1C       |ADD ECX,1C
	004D87ED  |.  83C7 04       |ADD EDI,4
	004D87F0  |.  8B5424 34     |MOV EDX,DWORD PTR SS:[ESP+34]
	004D87F4  |.  894424 24     |MOV DWORD PTR SS:[ESP+24],EAX
	004D87F8  |.  42            |INC EDX
	004D87F9  |.  33C0          |XOR EAX,EAX
	004D87FB  |.  894C24 14     |MOV DWORD PTR SS:[ESP+14],ECX
	004D87FF  |.  895424 34     |MOV DWORD PTR SS:[ESP+34],EDX
	004D8803  |.  8B4C24 4C     |MOV ECX,DWORD PTR SS:[ESP+4C]
	004D8807  |.  66:8B41 12    |MOV AX,WORD PTR DS:[ECX+12]
	004D880B  |.  48            |DEC EAX
	004D880C  |.  3BC2          |CMP EAX,EDX
	004D880E  |.^ 0F8F 61FFFFFF \JG 004D8775
	004D8814  |>  8B4424 18     MOV EAX,DWORD PTR SS:[ESP+18]
	004D8818  |.  8B6C24 20     MOV EBP,DWORD PTR SS:[ESP+20]
	004D881C  |.  C1E0 02       SHL EAX,2
	004D881F  |.  8B5424 1C     MOV EDX,DWORD PTR SS:[ESP+1C]
	004D8823  |.  8D0CC0        LEA ECX,[EAX*8+EAX]
	004D8826  |.  8B4424 34     MOV EAX,DWORD PTR SS:[ESP+34]
	004D882A  |.  8D3429        LEA ESI,[EBP+ECX]
	004D882D  |.  8B4C24 4C     MOV ECX,DWORD PTR SS:[ESP+4C]
	004D8831  |.  51            PUSH ECX                                 ; /Arg4
	004D8832  |.  8B0C82        MOV ECX,DWORD PTR DS:[EAX*4+EDX]         ; |
	004D8835  |.  51            PUSH ECX                                 ; |Arg3
	004D8836  |.  33C0          XOR EAX,EAX                              ; |
	004D8838  |.  66:8B46 1C    MOV AX,WORD PTR DS:[ESI+1C]              ; |
	004D883C  |.  50            PUSH EAX                                 ; |Arg2
	004D883D  |.  56            PUSH ESI                                 ; |Arg1
	004D883E  |.  E8 9D070000   CALL 004D8FE0                            ; \CARMA95.004D8FE0
	004D8843  |.  66:FF46 1C    INC WORD PTR DS:[ESI+1C]
	004D8847  |.  8B4C24 44     MOV ECX,DWORD PTR SS:[ESP+44]
	004D884B  |.  894D 10       MOV DWORD PTR SS:[EBP+10],ECX
	004D884E  |.  8B4C24 20     MOV ECX,DWORD PTR SS:[ESP+20]
	004D8852  |.  8B4424 2C     MOV EAX,DWORD PTR SS:[ESP+2C]
	004D8856  |.  8B5424 10     MOV EDX,DWORD PTR SS:[ESP+10]
	004D885A  |.  8D79 1E       LEA EDI,[ECX+1E]
	004D885D  |.  8941 14       MOV DWORD PTR DS:[ECX+14],EAX
	004D8860  |.  8B4424 4C     MOV EAX,DWORD PTR SS:[ESP+4C]
	004D8864  |.  8951 18       MOV DWORD PTR DS:[ECX+18],EDX
	004D8867  |.  8B4C24 28     MOV ECX,DWORD PTR SS:[ESP+28]
	004D886B  |.  33F6          XOR ESI,ESI
	004D886D  |.  66:C707 0100  MOV WORD PTR DS:[EDI],1
	004D8872  |.  50            PUSH EAX                                 ; /Arg4
	004D8873  |.  8B11          MOV EDX,DWORD PTR DS:[ECX]               ; |
	004D8875  |.  52            PUSH EDX                                 ; |Arg3
	004D8876  |.  56            PUSH ESI                                 ; |Arg2 => 0
	004D8877  |.  55            PUSH EBP                                 ; |Arg1
	004D8878  |.  E8 43060000   CALL 004D8EC0                            ; \CARMA95.004D8EC0
	004D887D  |.  8B4424 30     MOV EAX,DWORD PTR SS:[ESP+30]
	004D8881  |.  897424 14     MOV DWORD PTR SS:[ESP+14],ESI
	004D8885  |.  897424 1C     MOV DWORD PTR SS:[ESP+1C],ESI
	004D8889  |.  3BC6          CMP EAX,ESI
	004D888B  |.  0F8E E9000000 JLE 004D897A
	004D8891  |.  8B4424 10     MOV EAX,DWORD PTR SS:[ESP+10]
	004D8895  |.  8B6C24 44     MOV EBP,DWORD PTR SS:[ESP+44]
	004D8899  |.  83C0 02       ADD EAX,2
	004D889C  |.  83C5 20       ADD EBP,20
	004D889F  |.  8B7424 28     MOV ESI,DWORD PTR SS:[ESP+28]
	004D88A3  |.  894424 18     MOV DWORD PTR SS:[ESP+18],EAX
	004D88A7  |.  8B4424 2C     MOV EAX,DWORD PTR SS:[ESP+2C]
	004D88AB  |.  895C24 24     MOV DWORD PTR SS:[ESP+24],EBX
	004D88AF  |.  83C0 04       ADD EAX,4
	004D88B2  |.  894424 2C     MOV DWORD PTR SS:[ESP+2C],EAX
	004D88B6  |.  8B4424 30     MOV EAX,DWORD PTR SS:[ESP+30]
	004D88BA  |.  894424 30     MOV DWORD PTR SS:[ESP+30],EAX
	004D88BE  |.  894424 14     MOV DWORD PTR SS:[ESP+14],EAX
	004D88C2  |>  8B4C24 4C     /MOV ECX,DWORD PTR SS:[ESP+4C]
	004D88C6  |.  8B46 04       |MOV EAX,DWORD PTR DS:[ESI+4]
	004D88C9  |.  0FBF50 0E     |MOVSX EDX,WORD PTR DS:[EAX+0E]
	004D88CD  |.  8B49 0C       |MOV ECX,DWORD PTR DS:[ECX+0C]
	004D88D0  |.  8B06          |MOV EAX,DWORD PTR DS:[ESI]
	004D88D2  |.  0FBF40 0E     |MOVSX EAX,WORD PTR DS:[EAX+0E]
	004D88D6  |.  8D1492        |LEA EDX,[EDX*4+EDX]
	004D88D9  |.  90            |NOP
	004D88DA  |.  8D0480        |LEA EAX,[EAX*4+EAX]
	004D88DD  |.  90            |NOP
	004D88DE  |.  8B5CD1 08     |MOV EBX,DWORD PTR DS:[EDX*8+ECX+8]
	004D88E2  |.  8B44C1 08     |MOV EAX,DWORD PTR DS:[EAX*8+ECX+8]
	004D88E6  |.  3BD8          |CMP EBX,EAX
	004D88E8  |.  74 19         |JE SHORT 004D8903
	004D88EA  |.  896F 16       |MOV DWORD PTR DS:[EDI+16],EBP
	004D88ED  |.  83C7 24       |ADD EDI,24
	004D88F0  |.  8B4424 2C     |MOV EAX,DWORD PTR SS:[ESP+2C]
	004D88F4  |.  8B4C24 18     |MOV ECX,DWORD PTR SS:[ESP+18]
	004D88F8  |.  8947 F6       |MOV DWORD PTR DS:[EDI-0A],EAX
	004D88FB  |.  894F FA       |MOV DWORD PTR DS:[EDI-6],ECX
	004D88FE  |.  66:C707 0000  |MOV WORD PTR DS:[EDI],0
	004D8903  |>  8B4C24 1C     |MOV ECX,DWORD PTR SS:[ESP+1C]
	004D8907  |.  8D46 04       |LEA EAX,[ESI+4]
	004D890A  |.  894C24 10     |MOV DWORD PTR SS:[ESP+10],ECX
	004D890E  |.  50            |PUSH EAX
	004D890F  |.  56            |PUSH ESI
	004D8910  |.  FF5424 48     |CALL DWORD PTR SS:[ESP+48]
	004D8914  |.  83C4 08       |ADD ESP,8
	004D8917  |.  85C0          |TEST EAX,EAX
	004D8919  |.  74 43         |JZ SHORT 004D895E
	004D891B  |.  8B5C24 18     |MOV EBX,DWORD PTR SS:[ARG.6]
	004D891F  |.  8B4424 2C     |MOV EAX,DWORD PTR SS:[ARG.11]
	004D8923  |.  83C3 02       |ADD EBX,2
	004D8926  |.  83C0 04       |ADD EAX,4
	004D8929  |.  83C5 20       |ADD EBP,20
	004D892C  |.  8B4C24 4C     |MOV ECX,DWORD PTR SS:[ARG.19]
	004D8930  |.  895C24 18     |MOV DWORD PTR SS:[ARG.6],EBX
	004D8934  |.  51            |PUSH ECX                                ; /Arg4
	004D8935  |.  8B5C24 20     |MOV EBX,DWORD PTR SS:[ARG.8]            ; |
	004D8939  |.  8D4F E2       |LEA ECX,[EDI-1E]                        ; |
	004D893C  |.  43            |INC EBX                                 ; |
	004D893D  |.  894424 30     |MOV DWORD PTR SS:[ARG.12],EAX           ; |
	004D8941  |.  8B06          |MOV EAX,DWORD PTR DS:[ESI]              ; |
	004D8943  |.  895C24 20     |MOV DWORD PTR SS:[ARG.8],EBX            ; |
	004D8947  |.  66:8958 0C    |MOV WORD PTR DS:[EAX+0C],BX             ; |
	004D894B  |.  8B46 04       |MOV EAX,DWORD PTR DS:[ESI+4]            ; |
	004D894E  |.  50            |PUSH EAX                                ; |Arg3
	004D894F  |.  33C0          |XOR EAX,EAX                             ; |
	004D8951  |.  66:8B07       |MOV AX,WORD PTR DS:[EDI]                ; |
	004D8954  |.  50            |PUSH EAX                                ; |Arg2
	004D8955  |.  51            |PUSH ECX                                ; |Arg1
	004D8956  |.  E8 65050000   |CALL 004D8EC0                           ; \CARMA95.004D8EC0
	004D895B  |.  66:FF07       |INC WORD PTR DS:[EDI]
	004D895E  |>  66:8B4424 10  |MOV AX,WORD PTR SS:[ARG.4]
	004D8963  |.  8B0E          |MOV ECX,DWORD PTR DS:[ESI]
	004D8965  |.  66:8941 0C    |MOV WORD PTR DS:[ECX+0C],AX
	004D8969  |.  83C6 04       |ADD ESI,4
	004D896C  |.  FF4C24 30     |DEC DWORD PTR SS:[ARG.12]
	004D8970  |.^ 0F85 4CFFFFFF \JNZ 004D88C2
	004D8976  |.  8B5C24 24     MOV EBX,DWORD PTR SS:[ARG.9]
	004D897A  |>  8B4C24 14     MOV ECX,DWORD PTR SS:[ARG.5]
	004D897E  |.  8B4424 28     MOV EAX,DWORD PTR SS:[ARG.10]
	004D8982  |.  8B6C24 38     MOV EBP,DWORD PTR SS:[ARG.14]
	004D8986  |.  85ED          TEST EBP,EBP
	004D8988  |.  8B1488        MOV EDX,DWORD PTR DS:[ECX*4+EAX]
	004D898B  |.  66:8B4C24 1C  MOV CX,WORD PTR SS:[ARG.7]
	004D8990  |.  66:894A 0C    MOV WORD PTR DS:[EDX+0C],CX
	004D8994  |.  0F8E 8D000000 JLE 004D8A27
	004D899A  |.  896C24 10     MOV DWORD PTR SS:[ARG.4],EBP
	004D899E  |>  33C9          /XOR ECX,ECX
	004D89A0  |.  66:390B       |CMP WORD PTR DS:[EBX],CX
	004D89A3  |.  894C24 34     |MOV DWORD PTR SS:[ARG.13],ECX
	004D89A7  |.  76 6C         |JBE SHORT 004D8A15
	004D89A9  |.  33D2          |XOR EDX,EDX
	004D89AB  |>  8B73 F4       |/MOV ESI,DWORD PTR DS:[EBX-0C]
	004D89AE  |.  8B4424 44     ||MOV EAX,DWORD PTR SS:[ARG.17]
	004D89B2  |.  2BF0          ||SUB ESI,EAX
	004D89B4  |.  33FF          ||XOR EDI,EDI
	004D89B6  |.  C1FE 05       ||SAR ESI,5
	004D89B9  |.  8B43 F0       ||MOV EAX,DWORD PTR DS:[EBX-10]
	004D89BC  |.  66:8B3C08     ||MOV DI,WORD PTR DS:[ECX+EAX]
	004D89C0  |.  8B6B E8       ||MOV EBP,DWORD PTR DS:[EBX-18]
	004D89C3  |.  83C2 1C       ||ADD EDX,1C
	004D89C6  |.  83C1 02       ||ADD ECX,2
	004D89C9  |.  8D047F        ||LEA EAX,[EDI*2+EDI]
	004D89CC  |.  8B7C24 3C     ||MOV EDI,DWORD PTR SS:[ARG.15]
	004D89D0  |.  C1E0 04       ||SHL EAX,4
	004D89D3  |.  03F8          ||ADD EDI,EAX
	004D89D5  |.  66:8B47 0C    ||MOV AX,WORD PTR DS:[EDI+0C]
	004D89D9  |.  66:2BC6       ||SUB AX,SI
	004D89DC  |.  66:894415 E4  ||MOV WORD PTR SS:[EDX+EBP-1C],AX
	004D89E1  |.  8B6B E8       ||MOV EBP,DWORD PTR DS:[EBX-18]
	004D89E4  |.  66:8B47 1C    ||MOV AX,WORD PTR DS:[EDI+1C]
	004D89E8  |.  66:2BC6       ||SUB AX,SI
	004D89EB  |.  66:894415 E6  ||MOV WORD PTR SS:[EDX+EBP-1A],AX
	004D89F0  |.  8B6B E8       ||MOV EBP,DWORD PTR DS:[EBX-18]
	004D89F3  |.  66:8B7F 2C    ||MOV DI,WORD PTR DS:[EDI+2C]
	004D89F7  |.  8B4424 34     ||MOV EAX,DWORD PTR SS:[ARG.13]
	004D89FB  |.  66:2BFE       ||SUB DI,SI
	004D89FE  |.  40            ||INC EAX
	004D89FF  |.  66:897C15 E8  ||MOV WORD PTR SS:[EDX+EBP-18],DI
	004D8A04  |.  894424 34     ||MOV DWORD PTR SS:[ARG.13],EAX
	004D8A08  |.  33C0          ||XOR EAX,EAX
	004D8A0A  |.  8B6C24 34     ||MOV EBP,DWORD PTR SS:[ARG.13]
	004D8A0E  |.  66:8B03       ||MOV AX,WORD PTR DS:[EBX]
	004D8A11  |.  3BC5          ||CMP EAX,EBP
	004D8A13  |.^ 7F 96         |\JG SHORT 004D89AB
	004D8A15  |>  83C3 24       |ADD EBX,24
	004D8A18  |.  8B4424 10     |MOV EAX,DWORD PTR SS:[ARG.4]
	004D8A1C  |.  48            |DEC EAX
	004D8A1D  |.  894424 10     |MOV DWORD PTR SS:[ARG.4],EAX
	004D8A21  |.^ 0F85 77FFFFFF \JNZ 004D899E
	004D8A27  |>  8B7C24 3C     MOV EDI,DWORD PTR SS:[ARG.15]
	004D8A2B  |.  57            PUSH EDI
	004D8A2C  |.  E8 6F7F0000   CALL 004E09A0
	004D8A31  |.  83C4 04       ADD ESP,4
	004D8A34  |.  5D            POP EBP
	004D8A35  |.  5F            POP EDI
	004D8A36  |.  5E            POP ESI
	004D8A37  |.  5B            POP EBX
	004D8A38  |.  83C4 38       ADD ESP,38
	004D8A3B  \.  C2 0400       RETN 4
	*/
}























int m004D8A70(const void * a, const void * b) {
	/*
	004D8A70  /.  8B4424 04     MOV EAX,DWORD PTR SS:[ARG.1]
	004D8A74  |.  53            PUSH EBX
	004D8A75  |.  8B5C24 0C     MOV EBX,DWORD PTR SS:[ARG.2]
	004D8A79  |.  56            PUSH ESI
	004D8A7A  |.  8B08          MOV ECX,DWORD PTR DS:[EAX]
	004D8A7C  |.  66:8B41 0C    MOV AX,WORD PTR DS:[ECX+0C]
	004D8A80  |.  8B13          MOV EDX,DWORD PTR DS:[EBX]
	004D8A82  |.  66:8B52 0C    MOV DX,WORD PTR DS:[EDX+0C]
	004D8A86  |.  66:3BC2       CMP AX,DX
	004D8A89  |.  75 05         JNE SHORT 004D8A90
	004D8A8B  |.  33C0          XOR EAX,EAX
	004D8A8D  |.  5E            POP ESI
	004D8A8E  |.  5B            POP EBX
	004D8A8F  |.  C3            RETN
	004D8A90  |>  0FBFC0        MOVSX EAX,AX
	004D8A93  |.  8B0D 44165400 MOV ECX,DWORD PTR DS:[541644]
	004D8A99  |.  8D1C80        LEA EBX,[EAX*4+EAX]
	004D8A9C  |.  0FBFC2        MOVSX EAX,DX
	004D8A9F  |.  8B49 08       MOV ECX,DWORD PTR DS:[ECX+8]
	004D8AA2  |.  33D2          XOR EDX,EDX
	004D8AA4  |.  8D34D9        LEA ESI,[EBX*8+ECX]
	004D8AA7  |.  8D1C80        LEA EBX,[EAX*4+EAX]
	004D8AAA  |.  8D0CD9        LEA ECX,[EBX*8+ECX]
	004D8AAD  |>  D901          /FLD DWORD PTR DS:[ECX]
	004D8AAF  |.  D81E          |FCOMP DWORD PTR DS:[ESI]
	004D8AB1  |.  DFE0          |FSTSW AX
	004D8AB3  |.  F6C4 01       |TEST AH,01
	004D8AB6  |.  75 1C         |JNZ SHORT 004D8AD4                      ; Taken if ST<[ESI] in preceding FCOMP at 004D8AAF
	004D8AB8  |.  D901          |FLD DWORD PTR DS:[ECX]
	004D8ABA  |.  D81E          |FCOMP DWORD PTR DS:[ESI]
	004D8ABC  |.  DFE0          |FSTSW AX
	004D8ABE  |.  F6C4 41       |TEST AH,41
	004D8AC1  |.  74 19         |JZ SHORT 004D8ADC                       ; Taken if ST>[ESI] in preceding FCOMP at 004D8ABA
	004D8AC3  |.  83C6 04       |ADD ESI,4
	004D8AC6  |.  83C1 04       |ADD ECX,4
	004D8AC9  |.  42            |INC EDX
	004D8ACA  |.  83FA 03       |CMP EDX,3
	004D8ACD  |.^ 7C DE         \JL SHORT 004D8AAD
	004D8ACF  |.  33C0          XOR EAX,EAX
	004D8AD1  |.  5E            POP ESI
	004D8AD2  |.  5B            POP EBX
	004D8AD3  |.  C3            RETN
	004D8AD4  |>  B8 01000000   MOV EAX,1
	004D8AD9  |.  5E            POP ESI
	004D8ADA  |.  5B            POP EBX
	004D8ADB  |.  C3            RETN
	004D8ADC  |>  B8 FFFFFFFF   MOV EAX,-1
	004D8AE1  |.  5E            POP ESI
	004D8AE2  |.  5B            POP EBX
	004D8AE3  \.  C3            RETN
	*/
	return (*(int*)a - *(int*)b);
}

int m004D8C50(const void * a, const void * b) {
	/*
	004D8C50  /.  8B4424 04     MOV EAX,DWORD PTR SS:[ARG.1]
	004D8C54  |.  53            PUSH EBX
	004D8C55  |.  8B5C24 0C     MOV EBX,DWORD PTR SS:[ARG.2]
	004D8C59  |.  56            PUSH ESI
	004D8C5A  |.  57            PUSH EDI
	004D8C5B  |.  8B08          MOV ECX,DWORD PTR DS:[EAX]
	004D8C5D  |.  8B13          MOV EDX,DWORD PTR DS:[EBX]
	004D8C5F  |.  A1 44165400   MOV EAX,DWORD PTR DS:[541644]
	004D8C64  |.  0FBF5A 0E     MOVSX EBX,WORD PTR DS:[EDX+0E]
	004D8C68  |.  8B78 0C       MOV EDI,DWORD PTR DS:[EAX+0C]
	004D8C6B  |.  8D049B        LEA EAX,[EBX*4+EBX]
	004D8C6E  |.  0FBF59 0E     MOVSX EBX,WORD PTR DS:[ECX+0E]
	004D8C72  |.  8B74C7 08     MOV ESI,DWORD PTR DS:[EAX*8+EDI+8]
	004D8C76  |.  8D049B        LEA EAX,[EBX*4+EBX]
	004D8C79  |.  8B44C7 08     MOV EAX,DWORD PTR DS:[EAX*8+EDI+8]
	004D8C7D  |.  3BF0          CMP ESI,EAX
	004D8C7F  |.  73 09         JAE SHORT 004D8C8A
	004D8C81  |.  B8 01000000   MOV EAX,1
	004D8C86  |.  5F            POP EDI
	004D8C87  |.  5E            POP ESI
	004D8C88  |.  5B            POP EBX
	004D8C89  |.  C3            RETN
	004D8C8A  |>  76 09         JBE SHORT 004D8C95
	004D8C8C  |.  B8 FFFFFFFF   MOV EAX,-1
	004D8C91  |.  5F            POP EDI
	004D8C92  |.  5E            POP ESI
	004D8C93  |.  5B            POP EBX
	004D8C94  |.  C3            RETN
	004D8C95  |>  66:8B72 0C    MOV SI,WORD PTR DS:[EDX+0C]
	004D8C99  |.  66:8B41 0C    MOV AX,WORD PTR DS:[ECX+0C]
	004D8C9D  |.  66:3BF0       CMP SI,AX
	004D8CA0  |.  7D 09         JGE SHORT 004D8CAB
	004D8CA2  |.  B8 01000000   MOV EAX,1
	004D8CA7  |.  5F            POP EDI
	004D8CA8  |.  5E            POP ESI
	004D8CA9  |.  5B            POP EBX
	004D8CAA  |.  C3            RETN
	004D8CAB  |>  7E 09         JLE SHORT 004D8CB6
	004D8CAD  |.  B8 FFFFFFFF   MOV EAX,-1
	004D8CB2  |.  5F            POP EDI
	004D8CB3  |.  5E            POP ESI
	004D8CB4  |.  5B            POP EBX
	004D8CB5  |.  C3            RETN
	004D8CB6  |>  33F6          XOR ESI,ESI
	004D8CB8  |>  D902          /FLD DWORD PTR DS:[EDX]
	004D8CBA  |.  D819          |FCOMP DWORD PTR DS:[ECX]
	004D8CBC  |.  DFE0          |FSTSW AX
	004D8CBE  |.  F6C4 01       |TEST AH,01
	004D8CC1  |.  75 1D         |JNZ SHORT 004D8CE0                      ; Taken if ST<[ECX] in preceding FCOMP at 004D8CBA
	004D8CC3  |.  D902          |FLD DWORD PTR DS:[EDX]
	004D8CC5  |.  D819          |FCOMP DWORD PTR DS:[ECX]
	004D8CC7  |.  DFE0          |FSTSW AX
	004D8CC9  |.  F6C4 41       |TEST AH,41
	004D8CCC  |.  74 1B         |JZ SHORT 004D8CE9                       ; Taken if ST>[ECX] in preceding FCOMP at 004D8CC5
	004D8CCE  |.  83C1 04       |ADD ECX,4
	004D8CD1  |.  83C2 04       |ADD EDX,4
	004D8CD4  |.  46            |INC ESI
	004D8CD5  |.  83FE 03       |CMP ESI,3
	004D8CD8  |.^ 7C DE         \JL SHORT 004D8CB8
	004D8CDA  |.  33C0          XOR EAX,EAX
	004D8CDC  |.  5F            POP EDI
	004D8CDD  |.  5E            POP ESI
	004D8CDE  |.  5B            POP EBX
	004D8CDF  |.  C3            RETN
	004D8CE0  |>  B8 01000000   MOV EAX,1
	004D8CE5  |.  5F            POP EDI
	004D8CE6  |.  5E            POP ESI
	004D8CE7  |.  5B            POP EBX
	004D8CE8  |.  C3            RETN
	004D8CE9  |>  B8 FFFFFFFF   MOV EAX,-1
	004D8CEE  |.  5F            POP EDI
	004D8CEF  |.  5E            POP ESI
	004D8CF0  |.  5B            POP EBX
	004D8CF1  \.  C3            RETN
	*/
	return (*(int*)a - *(int*)b);
}

int m004D8AF0(const void * a, const void * b) {
	/*
	004D8AF0  /.  8B4424 04     MOV EAX,DWORD PTR SS:[ARG.1]
	004D8AF4  |.  83EC 04       SUB ESP,4
	004D8AF7  |.  53            PUSH EBX
	004D8AF8  |.  8B08          MOV ECX,DWORD PTR DS:[EAX]
	004D8AFA  |.  8B5C24 10     MOV EBX,DWORD PTR SS:[ARG.2]
	004D8AFE  |.  56            PUSH ESI
	004D8AFF  |.  57            PUSH EDI
	004D8B00  |.  A1 44165400   MOV EAX,DWORD PTR DS:[541644]
	004D8B05  |.  55            PUSH EBP
	004D8B06  |.  8B13          MOV EDX,DWORD PTR DS:[EBX]
	004D8B08  |.  0FBF5A 0E     MOVSX EBX,WORD PTR DS:[EDX+0E]
	004D8B0C  |.  8B78 0C       MOV EDI,DWORD PTR DS:[EAX+0C]
	004D8B0F  |.  8D049B        LEA EAX,[EBX*4+EBX]
	004D8B12  |.  0FBF59 0E     MOVSX EBX,WORD PTR DS:[ECX+0E]
	004D8B16  |.  8B74C7 08     MOV ESI,DWORD PTR DS:[EAX*8+EDI+8]
	004D8B1A  |.  8D049B        LEA EAX,[EBX*4+EBX]
	004D8B1D  |.  8B44C7 08     MOV EAX,DWORD PTR DS:[EAX*8+EDI+8]
	004D8B21  |.  3BF0          CMP ESI,EAX
	004D8B23  |.  73 0D         JAE SHORT 004D8B32
	004D8B25  |.  B8 01000000   MOV EAX,1
	004D8B2A  |.  5D            POP EBP
	004D8B2B  |.  5F            POP EDI
	004D8B2C  |.  5E            POP ESI
	004D8B2D  |.  5B            POP EBX
	004D8B2E  |.  83C4 04       ADD ESP,4
	004D8B31  |.  C3            RETN
	004D8B32  |>  76 0D         JBE SHORT 004D8B41
	004D8B34  |.  B8 FFFFFFFF   MOV EAX,-1
	004D8B39  |.  5D            POP EBP
	004D8B3A  |.  5F            POP EDI
	004D8B3B  |.  5E            POP ESI
	004D8B3C  |.  5B            POP EBX
	004D8B3D  |.  83C4 04       ADD ESP,4
	004D8B40  |.  C3            RETN
	004D8B41  |>  66:8B72 0C    MOV SI,WORD PTR DS:[EDX+0C]
	004D8B45  |.  66:8B79 0C    MOV DI,WORD PTR DS:[ECX+0C]
	004D8B49  |.  66:3BF7       CMP SI,DI
	004D8B4C  |.  74 7E         JE SHORT 004D8BCC
	004D8B4E  |.  0FBFEF        MOVSX EBP,DI
	004D8B51  |.  A1 44165400   MOV EAX,DWORD PTR DS:[541644]
	004D8B56  |.  8B58 08       MOV EBX,DWORD PTR DS:[EAX+8]
	004D8B59  |.  8D44AD 00     LEA EAX,[EBP*4+EBP]
	004D8B5D  |.  0FBFEE        MOVSX EBP,SI
	004D8B60  |.  8D3CC3        LEA EDI,[EAX*8+EBX]
	004D8B63  |.  8BF7          MOV ESI,EDI
	004D8B65  |.  8D44AD 00     LEA EAX,[EBP*4+EBP]
	004D8B69  |.  C74424 10 000 MOV DWORD PTR SS:[LOCAL.0],0
	004D8B71  |.  8D1CC3        LEA EBX,[EAX*8+EBX]
	004D8B74  |.  8BEB          MOV EBP,EBX
	004D8B76  |>  D906          /FLD DWORD PTR DS:[ESI]
	004D8B78  |.  D85D 00       |FCOMP DWORD PTR SS:[EBP]
	004D8B7B  |.  DFE0          |FSTSW AX
	004D8B7D  |.  F6C4 41       |TEST AH,41
	004D8B80  |.  74 78         |JZ SHORT 004D8BFA                       ; Taken if ST>[EBP] in preceding FCOMP at 004D8B78
	004D8B82  |.  D906          |FLD DWORD PTR DS:[ESI]
	004D8B84  |.  D85D 00       |FCOMP DWORD PTR SS:[EBP]
	004D8B87  |.  DFE0          |FSTSW AX
	004D8B89  |.  F6C4 01       |TEST AH,01
	004D8B8C  |.  75 79         |JNZ SHORT 004D8C07                      ; Taken if ST<[EBP] in preceding FCOMP at 004D8B84
	004D8B8E  |.  83C5 04       |ADD EBP,4
	004D8B91  |.  83C6 04       |ADD ESI,4
	004D8B94  |.  8B4424 10     |MOV EAX,DWORD PTR SS:[LOCAL.0]
	004D8B98  |.  40            |INC EAX
	004D8B99  |.  894424 10     |MOV DWORD PTR SS:[LOCAL.0],EAX
	004D8B9D  |.  83F8 03       |CMP EAX,3
	004D8BA0  |.^ 7C D4         \JL SHORT 004D8B76
	004D8BA2  |.  33F6          XOR ESI,ESI
	004D8BA4  |.  83C3 0C       ADD EBX,0C
	004D8BA7  |.  83C7 0C       ADD EDI,0C
	004D8BAA  |>  D907          /FLD DWORD PTR DS:[EDI]
	004D8BAC  |.  D81B          |FCOMP DWORD PTR DS:[EBX]
	004D8BAE  |.  DFE0          |FSTSW AX
	004D8BB0  |.  F6C4 41       |TEST AH,41
	004D8BB3  |.  74 5F         |JZ SHORT 004D8C14                       ; Taken if ST>[EBX] in preceding FCOMP at 004D8BAC
	004D8BB5  |.  D907          |FLD DWORD PTR DS:[EDI]
	004D8BB7  |.  D81B          |FCOMP DWORD PTR DS:[EBX]
	004D8BB9  |.  DFE0          |FSTSW AX
	004D8BBB  |.  F6C4 01       |TEST AH,01
	004D8BBE  |.  75 61         |JNZ SHORT 004D8C21                      ; Taken if ST<[EBX] in preceding FCOMP at 004D8BB7
	004D8BC0  |.  83C3 04       |ADD EBX,4
	004D8BC3  |.  83C7 04       |ADD EDI,4
	004D8BC6  |.  46            |INC ESI
	004D8BC7  |.  83FE 02       |CMP ESI,2
	004D8BCA  |.^ 7C DE         \JL SHORT 004D8BAA
	004D8BCC  |>  33F6          XOR ESI,ESI
	004D8BCE  |>  D902          /FLD DWORD PTR DS:[EDX]
	004D8BD0  |.  D819          |FCOMP DWORD PTR DS:[ECX]
	004D8BD2  |.  DFE0          |FSTSW AX
	004D8BD4  |.  F6C4 01       |TEST AH,01
	004D8BD7  |.  75 55         |JNZ SHORT 004D8C2E                      ; Taken if ST<[ECX] in preceding FCOMP at 004D8BD0
	004D8BD9  |.  D902          |FLD DWORD PTR DS:[EDX]
	004D8BDB  |.  D819          |FCOMP DWORD PTR DS:[ECX]
	004D8BDD  |.  DFE0          |FSTSW AX
	004D8BDF  |.  F6C4 41       |TEST AH,41
	004D8BE2  |.  74 57         |JZ SHORT 004D8C3B                       ; Taken if ST>[ECX] in preceding FCOMP at 004D8BDB
	004D8BE4  |.  83C1 04       |ADD ECX,4
	004D8BE7  |.  83C2 04       |ADD EDX,4
	004D8BEA  |.  46            |INC ESI
	004D8BEB  |.  83FE 03       |CMP ESI,3
	004D8BEE  |.^ 7C DE         \JL SHORT 004D8BCE
	004D8BF0  |.  33C0          XOR EAX,EAX
	004D8BF2  |.  5D            POP EBP
	004D8BF3  |.  5F            POP EDI
	004D8BF4  |.  5E            POP ESI
	004D8BF5  |.  5B            POP EBX
	004D8BF6  |.  83C4 04       ADD ESP,4
	004D8BF9  |.  C3            RETN
	004D8BFA  |>  B8 01000000   MOV EAX,1
	004D8BFF  |.  5D            POP EBP
	004D8C00  |.  5F            POP EDI
	004D8C01  |.  5E            POP ESI
	004D8C02  |.  5B            POP EBX
	004D8C03  |.  83C4 04       ADD ESP,4
	004D8C06  |.  C3            RETN
	004D8C07  |>  B8 FFFFFFFF   MOV EAX,-1
	004D8C0C  |.  5D            POP EBP
	004D8C0D  |.  5F            POP EDI
	004D8C0E  |.  5E            POP ESI
	004D8C0F  |.  5B            POP EBX
	004D8C10  |.  83C4 04       ADD ESP,4
	004D8C13  |.  C3            RETN
	004D8C14  |>  B8 01000000   MOV EAX,1
	004D8C19  |.  5D            POP EBP
	004D8C1A  |.  5F            POP EDI
	004D8C1B  |.  5E            POP ESI
	004D8C1C  |.  5B            POP EBX
	004D8C1D  |.  83C4 04       ADD ESP,4
	004D8C20  |.  C3            RETN
	004D8C21  |>  B8 FFFFFFFF   MOV EAX,-1
	004D8C26  |.  5D            POP EBP
	004D8C27  |.  5F            POP EDI
	004D8C28  |.  5E            POP ESI
	004D8C29  |.  5B            POP EBX
	004D8C2A  |.  83C4 04       ADD ESP,4
	004D8C2D  |.  C3            RETN
	004D8C2E  |>  B8 01000000   MOV EAX,1
	004D8C33  |.  5D            POP EBP
	004D8C34  |.  5F            POP EDI
	004D8C35  |.  5E            POP ESI
	004D8C36  |.  5B            POP EBX
	004D8C37  |.  83C4 04       ADD ESP,4
	004D8C3A  |.  C3            RETN
	004D8C3B  |>  B8 FFFFFFFF   MOV EAX,-1
	004D8C40  |.  5D            POP EBP
	004D8C41  |.  5F            POP EDI
	004D8C42  |.  5E            POP ESI
	004D8C43  |.  5B            POP EBX
	004D8C44  |.  83C4 04       ADD ESP,4
	004D8C47  \.  C3            RETN
	*/
	return (*(int*)a - *(int*)b);
}

int m004D8DB0(const void * a, const void * b) {
	/*
	004D8DB0  /.  8B5424 10     MOV EDX,DWORD PTR SS:[ARG.4]
	004D8DB4  |.  83EC 0C       SUB ESP,0C
	004D8DB7  |.  53            PUSH EBX
	004D8DB8  |.  56            PUSH ESI
	004D8DB9  |.  8B7424 20     MOV ESI,DWORD PTR SS:[ARG.3]
	004D8DBD  |.  57            PUSH EDI
	004D8DBE  |.  55            PUSH EBP
	004D8DBF  |.  3BF2          CMP ESI,EDX
	004D8DC1  |.  0F83 E8000000 JAE 004D8EAF
	004D8DC7  |.  8B4C24 20     MOV ECX,DWORD PTR SS:[ARG.1]
	004D8DCB  |>  8B06          /MOV EAX,DWORD PTR DS:[ESI]
	004D8DCD  |.  8B59 0C       |MOV EBX,DWORD PTR DS:[ECX+0C]
	004D8DD0  |.  0FBF78 0E     |MOVSX EDI,WORD PTR DS:[EAX+0E]
	004D8DD4  |.  8D3CBF        |LEA EDI,[EDI*4+EDI]
	004D8DD7  |.  8B44FB 18     |MOV EAX,DWORD PTR DS:[EDI*8+EBX+18]
	004D8DDB  |.  8B6CFB 20     |MOV EBP,DWORD PTR DS:[EDI*8+EBX+20]
	004D8DDF  |.  8D3CFB        |LEA EDI,[EDI*8+EBX]
	004D8DE2  |.  894424 10     |MOV DWORD PTR SS:[LOCAL.2],EAX
	004D8DE6  |.  896C24 18     |MOV DWORD PTR SS:[LOCAL.0],EBP
	004D8DEA  |.  90            |NOP
	004D8DEB  |.  8B5F 1C       |MOV EBX,DWORD PTR DS:[EDI+1C]
	004D8DEE  |.  8B7C24 28     |MOV EDI,DWORD PTR SS:[ARG.3]
	004D8DF2  |.  895C24 14     |MOV DWORD PTR SS:[LOCAL.1],EBX
	004D8DF6  |.  3BFA          |CMP EDI,EDX
	004D8DF8  |.  0F83 A6000000 |JAE 004D8EA4
	004D8DFE  |>  3BFE          |/CMP EDI,ESI
	004D8E00  |.  74 4A         ||JE SHORT 004D8E4C
	004D8E02  |.  8B59 0C       ||MOV EBX,DWORD PTR DS:[ECX+0C]
	004D8E05  |.  8B07          ||MOV EAX,DWORD PTR DS:[EDI]
	004D8E07  |.  0FBF68 0E     ||MOVSX EBP,WORD PTR DS:[EAX+0E]
	004D8E0B  |.  8D44AD 00     ||LEA EAX,[EBP*4+EBP]
	004D8E0F  |.  8D2CC3        ||LEA EBP,[EAX*8+EBX]
	004D8E12  |.  8B06          ||MOV EAX,DWORD PTR DS:[ESI]
	004D8E14  |.  0FBF40 0E     ||MOVSX EAX,WORD PTR DS:[EAX+0E]
	004D8E18  |.  8D0480        ||LEA EAX,[EAX*4+EAX]
	004D8E1B  |.  66:8B44C3 06  ||MOV AX,WORD PTR DS:[EAX*8+EBX+6]
	004D8E20  |.  66:8545 06    ||TEST WORD PTR SS:[EBP+6],AX
	004D8E24  |.  74 73         ||JZ SHORT 004D8E99
	004D8E26  |.  D945 1C       ||FLD DWORD PTR SS:[EBP+1C]
	004D8E29  |.  D84C24 14     ||FMUL DWORD PTR SS:[LOCAL.1]
	004D8E2D  |.  D945 18       ||FLD DWORD PTR SS:[EBP+18]
	004D8E30  |.  D84C24 10     ||FMUL DWORD PTR SS:[LOCAL.2]
	004D8E34  |.  D945 20       ||FLD DWORD PTR SS:[EBP+20]
	004D8E37  |.  D84C24 18     ||FMUL DWORD PTR SS:[LOCAL.0]
	004D8E3B  |.  D9C9          ||FXCH ST(1)
	004D8E3D  |.  DEC2          ||FADDP ST(2),ST
	004D8E3F  |.  DEC1          ||FADDP ST(1),ST
	004D8E41  |.  D85C24 24     ||FCOMP DWORD PTR SS:[ARG.2]
	004D8E45  |.  DFE0          ||FSTSW AX
	004D8E47  |.  F6C4 41       ||TEST AH,41
	004D8E4A  |.  75 4D         ||JNZ SHORT 004D8E99                     ; Taken if ST<=[ESP+24] in preceding FCOMP at 004D8E41
	004D8E4C  |>  8B1E          ||MOV EBX,DWORD PTR DS:[ESI]
	004D8E4E  |.  8B07          ||MOV EAX,DWORD PTR DS:[EDI]
	004D8E50  |.  0FBF68 0E     ||MOVSX EBP,WORD PTR DS:[EAX+0E]
	004D8E54  |.  8D44AD 00     ||LEA EAX,[EBP*4+EBP]
	004D8E58  |.  8B69 0C       ||MOV EBP,DWORD PTR DS:[ECX+0C]
	004D8E5B  |.  D944C5 18     ||FLD DWORD PTR SS:[EAX*8+EBP+18]
	004D8E5F  |.  D803          ||FADD DWORD PTR DS:[EBX]
	004D8E61  |.  D91B          ||FSTP DWORD PTR DS:[EBX]
	004D8E63  |.  8B1E          ||MOV EBX,DWORD PTR DS:[ESI]
	004D8E65  |.  8B07          ||MOV EAX,DWORD PTR DS:[EDI]
	004D8E67  |.  0FBF68 0E     ||MOVSX EBP,WORD PTR DS:[EAX+0E]
	004D8E6B  |.  83C3 04       ||ADD EBX,4
	004D8E6E  |.  90            ||NOP
	004D8E6F  |.  8D44AD 00     ||LEA EAX,[EBP*4+EBP]
	004D8E73  |.  8B69 0C       ||MOV EBP,DWORD PTR DS:[ECX+0C]
	004D8E76  |.  D944C5 1C     ||FLD DWORD PTR SS:[EAX*8+EBP+1C]
	004D8E7A  |.  D803          ||FADD DWORD PTR DS:[EBX]
	004D8E7C  |.  D91B          ||FSTP DWORD PTR DS:[EBX]
	004D8E7E  |.  8B1E          ||MOV EBX,DWORD PTR DS:[ESI]
	004D8E80  |.  8B07          ||MOV EAX,DWORD PTR DS:[EDI]
	004D8E82  |.  0FBF68 0E     ||MOVSX EBP,WORD PTR DS:[EAX+0E]
	004D8E86  |.  83C3 08       ||ADD EBX,8
	004D8E89  |.  90            ||NOP
	004D8E8A  |.  8D44AD 00     ||LEA EAX,[EBP*4+EBP]
	004D8E8E  |.  8B69 0C       ||MOV EBP,DWORD PTR DS:[ECX+0C]
	004D8E91  |.  D944C5 20     ||FLD DWORD PTR SS:[EAX*8+EBP+20]
	004D8E95  |.  D803          ||FADD DWORD PTR DS:[EBX]
	004D8E97  |.  D91B          ||FSTP DWORD PTR DS:[EBX]
	004D8E99  |>  83C7 04       ||ADD EDI,4
	004D8E9C  |.  3BFA          ||CMP EDI,EDX
	004D8E9E  |.^ 0F82 5AFFFFFF |\JB 004D8DFE
	004D8EA4  |>  83C6 04       |ADD ESI,4
	004D8EA7  |.  3BF2          |CMP ESI,EDX
	004D8EA9  |.^ 0F82 1CFFFFFF \JB 004D8DCB
	004D8EAF  |>  5D            POP EBP
	004D8EB0  |.  5F            POP EDI
	004D8EB1  |.  5E            POP ESI
	004D8EB2  |.  5B            POP EBX
	004D8EB3  |.  83C4 0C       ADD ESP,0C
	004D8EB6  \.  C2 1000       RETN 10
	*/
	return (*(int*)a - *(int*)b);
}

int m004D8D00(const void * a, const void * b) {
	/*
	004D8D00  /.  8B5424 10     MOV EDX,DWORD PTR SS:[ARG.4]
	004D8D04  |.  53            PUSH EBX
	004D8D05  |.  56            PUSH ESI
	004D8D06  |.  57            PUSH EDI
	004D8D07  |.  8B7424 18     MOV ESI,DWORD PTR SS:[ARG.3]
	004D8D0B  |.  55            PUSH EBP
	004D8D0C  |.  3BD6          CMP EDX,ESI
	004D8D0E  |.  0F86 91000000 JBE 004D8DA5
	004D8D14  |.  8B4C24 14     MOV ECX,DWORD PTR SS:[ARG.1]
	004D8D18  |>  8B7C24 1C     /MOV EDI,DWORD PTR SS:[ARG.3]
	004D8D1C  |.  3BD7          |CMP EDX,EDI
	004D8D1E  |.  76 7A         |JBE SHORT 004D8D9A
	004D8D20  |>  3BFE          |/CMP EDI,ESI
	004D8D22  |.  74 23         ||JE SHORT 004D8D47
	004D8D24  |.  8B59 0C       ||MOV EBX,DWORD PTR DS:[ECX+0C]
	004D8D27  |.  8B07          ||MOV EAX,DWORD PTR DS:[EDI]
	004D8D29  |.  0FBF68 0E     ||MOVSX EBP,WORD PTR DS:[EAX+0E]
	004D8D2D  |.  8D44AD 00     ||LEA EAX,[EBP*4+EBP]
	004D8D31  |.  8B2E          ||MOV EBP,DWORD PTR DS:[ESI]
	004D8D33  |.  0FBF6D 0E     ||MOVSX EBP,WORD PTR SS:[EBP+0E]
	004D8D37  |.  66:8B44C3 06  ||MOV AX,WORD PTR DS:[EAX*8+EBX+6]
	004D8D3C  |.  8D6CAD 00     ||LEA EBP,[EBP*4+EBP]
	004D8D40  |.  66:8544EB 06  ||TEST WORD PTR DS:[EBP*8+EBX+6],AX
	004D8D45  |.  74 4C         ||JZ SHORT 004D8D93
	004D8D47  |>  8B1E          ||MOV EBX,DWORD PTR DS:[ESI]
	004D8D49  |.  8B07          ||MOV EAX,DWORD PTR DS:[EDI]
	004D8D4B  |.  0FBF68 0E     ||MOVSX EBP,WORD PTR DS:[EAX+0E]
	004D8D4F  |.  8D44AD 00     ||LEA EAX,[EBP*4+EBP]
	004D8D53  |.  8B69 0C       ||MOV EBP,DWORD PTR DS:[ECX+0C]
	004D8D56  |.  D944C5 18     ||FLD DWORD PTR SS:[EAX*8+EBP+18]
	004D8D5A  |.  D803          ||FADD DWORD PTR DS:[EBX]
	004D8D5C  |.  D91B          ||FSTP DWORD PTR DS:[EBX]
	004D8D5E  |.  8B1E          ||MOV EBX,DWORD PTR DS:[ESI]
	004D8D60  |.  8B07          ||MOV EAX,DWORD PTR DS:[EDI]
	004D8D62  |.  0FBF68 0E     ||MOVSX EBP,WORD PTR DS:[EAX+0E]
	004D8D66  |.  83C3 04       ||ADD EBX,4
	004D8D69  |.  8B41 0C       ||MOV EAX,DWORD PTR DS:[ECX+0C]
	004D8D6C  |.  8D6CAD 00     ||LEA EBP,[EBP*4+EBP]
	004D8D70  |.  D944E8 1C     ||FLD DWORD PTR DS:[EBP*8+EAX+1C]
	004D8D74  |.  D803          ||FADD DWORD PTR DS:[EBX]
	004D8D76  |.  D91B          ||FSTP DWORD PTR DS:[EBX]
	004D8D78  |.  8B1E          ||MOV EBX,DWORD PTR DS:[ESI]
	004D8D7A  |.  8B07          ||MOV EAX,DWORD PTR DS:[EDI]
	004D8D7C  |.  0FBF68 0E     ||MOVSX EBP,WORD PTR DS:[EAX+0E]
	004D8D80  |.  83C3 08       ||ADD EBX,8
	004D8D83  |.  90            ||NOP
	004D8D84  |.  8D44AD 00     ||LEA EAX,[EBP*4+EBP]
	004D8D88  |.  8B69 0C       ||MOV EBP,DWORD PTR DS:[ECX+0C]
	004D8D8B  |.  D944C5 20     ||FLD DWORD PTR SS:[EAX*8+EBP+20]
	004D8D8F  |.  D803          ||FADD DWORD PTR DS:[EBX]
	004D8D91  |.  D91B          ||FSTP DWORD PTR DS:[EBX]
	004D8D93  |>  83C7 04       ||ADD EDI,4
	004D8D96  |.  3BFA          ||CMP EDI,EDX
	004D8D98  |.^ 72 86         |\JB SHORT 004D8D20
	004D8D9A  |>  83C6 04       |ADD ESI,4
	004D8D9D  |.  3BF2          |CMP ESI,EDX
	004D8D9F  |.^ 0F82 73FFFFFF \JB 004D8D18
	004D8DA5  |>  5D            POP EBP
	004D8DA6  |.  5F            POP EDI
	004D8DA7  |.  5E            POP ESI
	004D8DA8  |.  5B            POP EBX
	004D8DA9  \.  C2 1000       RETN 10
	*/
	return (*(int*)a - *(int*)b);
}