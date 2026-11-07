
/*
 *   Source [main.c]
 */

#include "rose.h"

typedef uint8_t  uInt8;
typedef uint16_t uInt16;
typedef uint32_t uInt32;
typedef uint64_t uInt64;

typedef int8_t  Int8;
typedef int16_t Int16;
typedef int32_t Int32;
typedef int64_t Int64;

#define COLOR_RED_CHANNEL()
#define COLOR_GREEN_CHANNEL()
#define COLOR_BLUE_CHANNEL()
#define COLOR_ALPHA_CHANNEL()

/* 
 * 
 * caller is responsible for freeing memory allocated to image_result.
 * each pixel is an unsigned 32 bit integer.
 * packing:
 * 
 *  RED      GREEN    BLUE     ALPHA
 *  00000000 00000000 00000000 00000000 = uInt32
 *  ^                 <------>        ^
 *  Most Sig bit       1 byte         Least Sig Bit
 * 
 * Each channel is, of course, 0 - 255
 * 
 * It should be possible to cast one of these pixels (uInt32) to
 * a struct that looks like this:
 * typedef struct RGBA { uInt8 r, g, b, a; }
 * 
 * Memory Layout:
 * 
 * //indexing a specific pixel should look like:
 * uInt32 index = ypos * width + xpos;
 * uInt32 pixel = pixels[index];
 * 
 * Coordinate (0, 0) is top-left corner of image. y-axis grows down, x-axis grows to the right.
 * 
 * Memory layout (visually explained):
 * 
 * an illustrative 6x6 pixel image:
 * 
 *   Top Left Corner (0, 0)
 *   v
 *   * * * * * *
 *   * * * * * *
 *   * * * * * *
 *   * * * * * *
 *   * * * * * *
 *   * * * * * *
 * 
 *     Top Left Corner (0, 0)
 *     v
 *   [ * * * * * * ] <- row 0
 *   [ * * * * * * ] <- row 1
 *     * * * * * *
 *     * * * * * *
 *     * * * * * *
 *   [ * * * * * * ] <- row 5
 * 
 *     Top Left Corner (0, 0)
 *     v
 *     * * * * * *
 *     * * * * * *
 *     * * * * * *
 *     * * * * * *
 *     * * * * * *
 *     * * * * * *
 *     ^         ^
 *     |         |
 *     |         column 5
 *     |
 *     column 0
 * 
 * 
 * uInt32* pixels = [ ... row 0 ... ][ ... row 1 ... ][ ... row 2 ... ][ ... row 3 ... ][ ... row 4 ... ][ ... row 5 ... ]
 * 
 * 
 * png_decoder should return an error code and leave 'image_result', 'width_result', and 'height_result' unmodified
 * in the event of any inability to decode the image for any reason.
 * 
 * png_decoder should return error if image layout isn't already in the form R8G8B8A8 - this is intended to be a pretty simple utility,
 * not necessarily production-grade for every possible image type (even if they're technically valid).
 * 
 * Don't use any external libraries, except for the C standard library (libc).
 * 
 * if you need/want to add static helper functions, macro definitions, enums, and/or structs (only to be used/called by 'png_decoder', of course),
 * you may do so only in the region below.
 * 
 * A (skeleton) that converts an integer return status into a (slightly) more helpful diagnostic message is already provided inside
 * of your allowed helper area. This function should be filled out - but doesnt need to be super verbose - exercise reasonable judgement.
 * 
 * Dont edit any code that isnt inside <HELPER BEGIN> ... <HELPER END> or inside the body of png_decoder() { ... }
 * 
 **/

/* <HELPER BEGIN> */

const char* png_error(int return_status) // please fill this out too
{
	switch (return_status) {
		case (1): return "out of memory"; // just an example - maybe another mapping - i.e. to different integer - or different wording works better if you want to
		default: return "unknown";
	}
}

/* <HELPER END> */


int png_decoder(const char* path, uInt32** image_result, uInt32* width_result, uInt32* height_result)
{
	// open file pointed to by the directory path contained in 'path' string
	// read bytes from file into array of bytes uInt8* data = ... (free at end of function)
	// decode the png's header - 
	// decode png from byte array
	// ...
	// 
	// Collect results:
	//*image_result = ...
	//*width_result = ...
	//*height_result = ...
	return 0; // stubbed for now - png_decoder() doesnt do anything yet
}

int main(void)
{
	ROSE_Init("PNG Decoder Demo", 1600, 900);
	ROSE_WindowVSync(true);

	/*
	 * 
	 * 'ROSE_Image' is a user created type that wraps the stbi_image api - your task doesnt need to touch ROSE_Image (and really shouldn't).
	 * My hope is that I can replace my reliance on stbi_image with png_decoder - but you just need to worry about the raw implementation, I
	 * will do the wrapping into an API that I like at a later time.
	 * 
	 * Below, here in 'int main(void)', I've set up a simple test bench.
	 * All it does is let me compare the two images against eachother visually.
	 * Both are rendered as ROSE_Sprites, but one if loaded from stbi_image and the other is loaded from png_decoder.
	 * 
	 */

	const char* path = "test_image.png";
	uInt32* pixels = NULL;
	uInt32 width = 0, height = 0;
	int return_status = png_decoder(path, &pixels, &width, &height);

	if (return_status != 0) {
		fprintf(stderr, "png_decoder() failed! (error: %d) message: %s\n", png_error(return_status));
		exit(EXIT_FAILURE);
	}

	//ROSE_Image image_decoded = ROSE_ImageCreate(width, height);
	ROSE_Image image_stbi = ROSE_ImageLoadFromFile(path);

	//for (uInt32 y = 0; y < image_decoded._height; y++) {
	//	for (uInt32 x = 0; x < image_decoded._width; x++) {
	//		uInt32 index = y * image_decoded._width + x;
	//		uInt32 pixel = pixels[index];
	//		ROSE_RGBA color = (ROSE_RGBA)pixel;
	//		ROSE_ImageSetPixelRGBA(&image_decoded, x, y, color);
	//	}
	//}

	//ROSE_Sprite sprite_decoded = ROSE_SpriteCreate(&image_decoded);
	ROSE_Sprite sprite_stbi = ROSE_SpriteCreate(&image_stbi);
	sprite_stbi.xpos = 20;
	sprite_stbi.ypos = 20;
	sprite_stbi.scale = 3;

	while (ROSE_WindowPollEvents())
	{
		ROSE_WindowBeginFrame(ROSE_COLOR_BLUE);
		ROSE_SpriteDraw(&sprite_stbi);
		ROSE_WindowEndFrame();
	}

	//ROSE_SpriteDestroy(&sprite_decoded);
	ROSE_SpriteDestroy(&sprite_stbi);
	
	//ROSE_ImageDestroy(&image_decoded);
	ROSE_ImageDestroy(&image_stbi);

	ROSE_Quit();
	return EXIT_SUCCESS;
}

