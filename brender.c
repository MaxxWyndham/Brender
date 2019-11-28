#include "brender.h"
#include <stdarg.h>

#include <SDL.h>
#include <SDL_opengl.h>

br_list *reg_models;
br_list *reg_materials;
br_list *reg_textures;
br_list *reg_tables;

br_material	*DEFAULT_MATERIAL;
br_model	*DEFAULT_MODEL;

br_pixelmap *screen;
br_pixelmap *buffer;

void debug_printf(const char *fmt, const char *fn, const char *fmt2, ...)
{
	va_list ap;

	fprintf(stderr, fmt, fn);

	va_start(ap, fmt2);
	vfprintf(stderr, fmt2, ap);
	va_end(ap);

	fputs("\n", stderr);
}

void fatal_printf(const char *fmt, const char *fn, const char *fmt2, ...)
{
	va_list ap;

	fprintf(stderr, fmt, fn);

	va_start(ap, fmt2);
	vfprintf(stderr, fmt2, ap);
	va_end(ap);

	fputs("\n", stderr);

	exit(1);
}

void BrBegin(void)
{
	/* only init timer, audio and video sub systems are initialized separately */
	SDL_InitSubSystem(SDL_INIT_TIMER);
	// | SDL_INIT_NOPARACHUTE

	reg_models = BrMemCalloc(1, sizeof(br_list), BR_MEMORY_REGISTRY);
	reg_models->head = NULL;

	reg_materials = BrMemCalloc(1, sizeof(br_list), BR_MEMORY_REGISTRY);
	reg_materials->head = NULL;

	reg_textures = BrMemCalloc(1, sizeof(br_list), BR_MEMORY_REGISTRY);
	reg_textures->head = NULL;

	reg_tables = BrMemCalloc(1, sizeof(br_list), BR_MEMORY_REGISTRY);
	reg_tables->head = NULL;

	DEFAULT_MATERIAL = BrDefaultMaterial();
	BrMaterialAdd(DEFAULT_MATERIAL);

	DEFAULT_MODEL = BrDefaultModel();
	BrModelAdd(DEFAULT_MODEL);
}

void BrEnd(void)
{
	BrModelRemove(DEFAULT_MODEL);
	BrModelFree(DEFAULT_MODEL);

	BrMaterialRemove(DEFAULT_MATERIAL);
	BrMaterialFree(DEFAULT_MATERIAL);

	BrListFree(reg_models);
	BrListFree(reg_materials);
	BrListFree(reg_textures);
	BrListFree(reg_tables);

	SDL_Quit();
}

br_uint_8 BrOnScreenCheck(const br_bounds* bounds)
{
	FIXME("(%p): stub", bounds);
	return 0;
}

void BrZbBegin(br_uint_8 colour_type, br_uint_8 depth_type)
{
	static br_matrix4 mat;

	//glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

	glViewport(0, 0, 640, 480);
}

void BrZbSceneRenderBegin(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer, br_pixelmap* depth_buffer)
{
	static br_matrix4 mat, mc;

	screen = colour_buffer;
	buffer = depth_buffer;

	glMatrixMode(GL_PROJECTION);
	BrMatrix4Identity(&mat);
	BrExtGLLoadMatrix(&mat);

    BrMatrix4Perspective(&mat, BR_ANGLE_DEG(55), BR_DIV(640, 480), -0.01f, -15.0f);
	BrExtGLMultMatrix(&mat);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	BrMatrix4Identity(&mat);
	BrExtGLLoadMatrix(&mat);

    BrMatrix4Identity(&mc);
	BrMatrix4Copy34(&mc, &camera->t.t.mat);
	BrMatrix4Inverse(&mat, &mc);

	BrExtGLMultMatrix(&mat);

//	GLfloat m[16];
//	glGetFloatv(GL_PROJECTION_MATRIX, m);
//
//	INFO("%f %f %f %f", m[0], m[4], m[8] , m[12]);
//	INFO("%f %f %f %f", m[1], m[5], m[9] , m[13]);
//	INFO("%f %f %f %f", m[2], m[6], m[10], m[14]);
//	INFO("%f %f %f %f", m[3], m[7], m[11], m[15]);
//	INFO("===");
}

void BrZbSceneRenderAdd(br_actor* world)
{
	br_uint_8 b = 1;
	br_actor *p = world;
	br_matrix4 mat;

	BrMatrix4Identity(&mat);

	INFO("Start");

	/* recalculating depth */
	while (b)
	{
		if (p->children) {
			p = p->children;
		} else {
			if (p->next) {
				p = p->next;
			} else {
				p = p->parent->next;
			}
		}

		if (p == NULL) { b = 0; } else {
			if (p->render_style == BR_RSTYLE_NONE) { continue; }

			if (p->t.type == BR_TRANSFORM_MATRIX34)
			{
				BrMatrix4Copy34(&mat, &p->t.t.mat);
				BrExtGLMultMatrix(&mat);
			}

			if (p->type == BR_ACTOR_MODEL)
			{
				br_model *m = (br_model *)p->model;
//				if (m->identifier[0] != '&') { continue; }
//				if (m->identifier[1] != '3') { continue; }
//				if (m->identifier[3] != '1') { continue; }

				if (m->prepared) {
					glBegin(GL_TRIANGLES);

					INFO("Drawing model \"%s\"", m->identifier);

					for (br_uint_32 k = 0; k < m->prepared->ngroups; k++)
					{
						for (int j = 0; j < m->prepared->groups[k].nfaces; j++)
						{
							//						INFO("%f %f %f", m->prepared->groups[k].vertices[m->prepared->groups[k].faces[j].vertices[0]].p.v[0], m->prepared->groups[k].vertices[m->prepared->groups[k].faces[j].vertices[0]].p.v[1], m->prepared->groups[k].vertices[m->prepared->groups[k].faces[j].vertices[0]].p.v[2]);
							//						INFO("%f %f %f", m->prepared->groups[k].vertices[m->prepared->groups[k].faces[j].vertices[1]].p.v[0], m->prepared->groups[k].vertices[m->prepared->groups[k].faces[j].vertices[1]].p.v[1], m->prepared->groups[k].vertices[m->prepared->groups[k].faces[j].vertices[1]].p.v[2]);
							//						INFO("%f %f %f", m->prepared->groups[k].vertices[m->prepared->groups[k].faces[j].vertices[2]].p.v[0], m->prepared->groups[k].vertices[m->prepared->groups[k].faces[j].vertices[2]].p.v[1], m->prepared->groups[k].vertices[m->prepared->groups[k].faces[j].vertices[2]].p.v[2]);
							glColor3f(1, 1, 1); glVertex3f(m->prepared->groups[k].vertices[m->prepared->groups[k].faces[j].vertices[0]].p.v[0], m->prepared->groups[k].vertices[m->prepared->groups[k].faces[j].vertices[0]].p.v[1], m->prepared->groups[k].vertices[m->prepared->groups[k].faces[j].vertices[0]].p.v[2]);
							glColor3f(1, 1, 1); glVertex3f(m->prepared->groups[k].vertices[m->prepared->groups[k].faces[j].vertices[1]].p.v[0], m->prepared->groups[k].vertices[m->prepared->groups[k].faces[j].vertices[1]].p.v[1], m->prepared->groups[k].vertices[m->prepared->groups[k].faces[j].vertices[1]].p.v[2]);
							glColor3f(1, 1, 1); glVertex3f(m->prepared->groups[k].vertices[m->prepared->groups[k].faces[j].vertices[2]].p.v[0], m->prepared->groups[k].vertices[m->prepared->groups[k].faces[j].vertices[2]].p.v[1], m->prepared->groups[k].vertices[m->prepared->groups[k].faces[j].vertices[2]].p.v[2]);
						}
					}

					glEnd();
				}
			}
		}
	}
}

void BrZbSceneRenderEnd(void)
{
	glClear(GL_DEPTH_BUFFER_BIT);

	glFlush();
	glFinish();
}

void BrZbSceneRender(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer, br_pixelmap* depth_buffer)
{
	BrZbSceneRenderBegin(world, camera, colour_buffer, depth_buffer);
	BrZbSceneRenderAdd(world);
	BrZbSceneRenderEnd();
}

void BrZbModelRender(br_actor* actor, br_model* model, const br_material* material, br_uint_8 style, int on_screen, int use_custom)
{
	FIXME("(%p, %p, %p, %d, %d, %d): stub", actor, model, material, style, on_screen, use_custom);
}

void BrZbEnd(void)
{
	FIXME("(void): stub");
}

void BrExtGLLoadMatrix(br_matrix4* mat)
{
    /* WRAPPER */
    /* Converts and transposes br_matrix4 to OpenGL matrix format and layout */
    GLfloat m[16] = {
                        mat->m[0][0], mat->m[0][1], mat->m[0][2], mat->m[0][3],
                        mat->m[1][0], mat->m[1][1], mat->m[1][2], mat->m[1][3],
                        mat->m[2][0], mat->m[2][1], mat->m[2][2], mat->m[2][3],
                        mat->m[3][0], mat->m[3][1], mat->m[3][2], mat->m[3][3]
                    };
    glLoadMatrixf(m);
}

void BrExtGLMultMatrix(br_matrix4* mat)
{
    /* WRAPPER */
    /* Converts and transposes br_matrix4 to OpenGL matrix format and layout */
    GLfloat m[16] = {
                        mat->m[0][0], mat->m[0][1], mat->m[0][2], mat->m[0][3],
                        mat->m[1][0], mat->m[1][1], mat->m[1][2], mat->m[1][3],
                        mat->m[2][0], mat->m[2][1], mat->m[2][2], mat->m[2][3],
                        mat->m[3][0], mat->m[3][1], mat->m[3][2], mat->m[3][3]
                    };
    glMultMatrixf(m);
}
