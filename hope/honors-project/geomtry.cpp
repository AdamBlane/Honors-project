//
////
////Heightmap *new_heightmap(unsigned width, unsigned depth, float step) 
////{
////	Heightmap *h = g_new0(TerTerrain, 1);
////	h->width = width;
////	h->depth = depth;
////	h->step = step;
////	h->height = g_new0(float, width * depth);
////	return h;
////}
//
//void set_heights_from_tex(Heightmap *h, int texture,float offset, float scale)
//{
//	TerTextureManager *tex_mgr =
//		(TerTextureManager *)ter_cache_get("textures/manager");
//
//	SDL_Surface *image = ter_texture_manager_get_image(tex_mgr, texture);
//	uint8_t *pixels = (uint8_t *)image->pixels;
//	float scale_x = ((float)image->w) / (t->width - 1);
//	float scale_z = ((float)image->h) / (t->depth - 1);
//
//	for (int x = 0; x < t->width; x++) {
//		for (int z = 0; z < t->depth; z++) {
//			int img_x = (int)truncf(x * scale_x);
//			int img_y = (int)truncf(z * scale_z);
//			float h = pixels[img_y * image->pitch + img_x * 4];
//
//			/* Normalize height to [-1, 1] */
//			h = h / 127.5 - 1.0f;
//
//			/* Apply scale */
//			h *= scale;
//
//			/* Apply height offset */
//			h += offset;
//
//			ter_terrain_set_height(t, x, z, h);
//		}
//	}
//}