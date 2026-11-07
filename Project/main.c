
/*
 *   Source [main.c]
 */

#include "rose.h"
#include "png.h"

int main(void)
{
	ROSE_Init("PNG Decoder Demo", 1600, 900);
	ROSE_WindowVSync(true);

	const char* path = "test_gray.png";
	uint32_t* pixels = NULL;
	uint32_t width = 0, height = 0;
	int return_status = png_decode(path, &pixels, &width, &height);

	if (return_status != 0) {
		fprintf(stderr, "png_decode() failed! (error: %d) message: %s\n", return_status, png_error(return_status));
		exit(EXIT_FAILURE);
	}

	ROSE_Image image_decoded = ROSE_ImageCreate(width, height);
	ROSE_Image image_stbi = ROSE_ImageLoadFromFile(path);

	for (uint32_t y = 0; y < image_decoded._height; y++) {
		for (uint32_t x = 0; x < image_decoded._width; x++) {
			uint32_t index = y * image_decoded._width + x;
			uint32_t pixel = pixels[index];
			uint8_t r = (uint8_t)(pixel >> 24);
			uint8_t g = (uint8_t)(pixel >> 16);
			uint8_t b = (uint8_t)(pixel >> 8);
			uint8_t a = (uint8_t)(pixel >> 0);
			ROSE_RGBA color = { r, g, b, a };
			ROSE_ImageSetPixelRGBA(&image_decoded, x, y, color);
		}
	}

	const uint32_t sprite_scale = 1;

	ROSE_Sprite sprite_stbi = ROSE_SpriteCreate(&image_stbi);
	sprite_stbi.xpos = 20;
	sprite_stbi.ypos = 20;
	sprite_stbi.scale = sprite_scale;
	
	ROSE_Sprite sprite_decoded = ROSE_SpriteCreate(&image_decoded);
	sprite_decoded.xpos = 40 + (sprite_stbi._width * sprite_scale);
	sprite_decoded.ypos = 20;
	sprite_decoded.scale = sprite_scale;

	while (ROSE_WindowPollEvents())
	{
		ROSE_WindowBeginFrame(ROSE_COLOR_BLUE);
		ROSE_SpriteDraw(&sprite_decoded);
		ROSE_SpriteDraw(&sprite_stbi);
		ROSE_WindowEndFrame();
	}

	ROSE_SpriteDestroy(&sprite_decoded);
	ROSE_SpriteDestroy(&sprite_stbi);
	
	ROSE_ImageDestroy(&image_decoded);
	ROSE_ImageDestroy(&image_stbi);

	ROSE_Quit();
	return EXIT_SUCCESS;
}

