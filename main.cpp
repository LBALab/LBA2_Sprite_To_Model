// Program to convert LBA2 sprite file to LBA2 model file.

// Libraries.
#include <iostream>

// Macros.
#define MAX 65536
#define SCL 0x32

// Variables.
FILE *fileIn;
FILE *fileOut;

char length;
char width;

char halfLength;
char halfWidth;

short offStart;
short offEnd;

short pixels;
short pixelX;
short pixelY;

short spheres;
short sphereSize;

char fileName[20];

char values[5] =
	{
	0x00,
	0x01,
	0x60,
	0x68,
	0xFF,
	};

// Functions.
void loadSpriteCreateModel(char fileName[20])
	{
	printf("Loading sprite %s. \n", fileName);
	
	fileIn = fopen(fileName, "rb");
	
	printf("Getting half length... \n");
	
	// Get Half Length.
	fseek(fileIn, 0x4, SEEK_SET);
	fread(&halfLength, sizeof(char), 0x1, fileIn);
	
	printf("Getting half width... \n");
	
	// Get Half Width.
	fseek(fileIn, 0x5, SEEK_SET);
	fread(&halfWidth, sizeof(char), 0x1, fileIn);
	
	printf("Getting length... \n");
	
	// Get Length.
	fseek(fileIn, 0x8, SEEK_SET);
	fread(&length, sizeof(char), 0x1, fileIn);
	
	printf("Getting width... \n");
	
	// Get Width.
	fseek(fileIn, 0x9, SEEK_SET);
	fread(&width, sizeof(char), 0x1, fileIn);
	
	printf("Getting pixels... \n");
	
	// Get All Pixels.
	pixels = (length * width);
	
	printf("Getting colors... \n");
	
	// Get All Colors.
	char colors[pixels];
	
	// Get Pixels.
	fseek(fileIn, 0xC, SEEK_SET);
	
	// Get colors.
	for (int i = 1; i <= pixels; i++)
		{
		fseek(fileIn, 0x0, SEEK_CUR);
		fread(&colors[i], sizeof(char), 0x1, fileIn);
		};
	
	fclose(fileIn);
	
	printf("Creating model... \n");
	
	fileOut = fopen("newTest.lun", "wb");
	
	printf("Listing bones... \n");
	
	// Bones Number.
	fseek(fileOut, 0x20, SEEK_SET);
	fwrite(&values[1], sizeof(char), 0x1, fileOut);
	
	printf("Listing bones offset... \n");
	
	// Bones Offset.
	fseek(fileOut, 0x24, SEEK_SET);
	fwrite(&values[2], sizeof(char), 0x1, fileOut);
	
	printf("Listing vertices... \n");
	
	pixels = (pixels + 1);
	
	// Vertices Number.
	fseek(fileOut, 0x28, SEEK_SET);
	fwrite(&pixels, sizeof(short), 0x1, fileOut);
	
	pixels = (pixels - 1);
	
	printf("Listing vertices offset... \n");
	
	// Vertices Offset.
	fseek(fileOut, 0x2C, SEEK_SET);
	fwrite(&values[3], sizeof(char), 0x1, fileOut);
	
	printf("Listing spheres... \n");
	
	// Spheres Number.
	fseek(fileOut, 0x50, SEEK_SET);
	fwrite(&pixels, sizeof(short), 0x1, fileOut);
	
	printf("Listing spheres offset... \n");
	
	// Spheres Offset.
	fseek(fileOut, 0x54, SEEK_SET);
	fwrite(&spheres, sizeof(short), 0x1, fileOut);
	
	// Bones.
	fseek(fileOut, 0x60, SEEK_SET);
	fwrite(&values[4], sizeof(char), 0x1, fileOut);
	
	fseek(fileOut, 0x61, SEEK_SET);
	fwrite(&values[4], sizeof(char), 0x1, fileOut);
	
	pixels = (pixels + 1);
	
	fseek(fileOut, 0x64, SEEK_SET);
	fwrite(&pixels, sizeof(short), 0x1, fileOut);
	
	pixels = (pixels - 1);
	
	fseek(fileOut, 0x66, SEEK_SET);
	fwrite(&values[0], sizeof(char), 0x1, fileOut);
	
	fseek(fileOut, 0x67, SEEK_SET);
	fwrite(&values[0], sizeof(char), 0x1, fileOut);
	
	fseek(fileOut, 0x68, SEEK_SET);
	
	// First Vertex.
	for (int i = 1; i <= 8; i++)
		{
		fseek(fileOut, 0x0, SEEK_CUR);
		fwrite(&values[0], sizeof(char), 0x1, fileOut);
		};
	
	// Set Height.
	pixelY = (width * SCL);
	
	offStart = 0x70;
	offEnd = 0x70;
	
	// All Other Vertices.
	for (int i = 1; i <= pixels; i++)
		{
		// X
		fseek(fileOut, 0x0, SEEK_CUR);
		fwrite(&pixelX, sizeof(short), 0x1, fileOut);
		
		// Y
		fseek(fileOut, 0x0, SEEK_CUR);
		fwrite(&pixelY, sizeof(short), 0x1, fileOut);
		
		// Z And Bone
		for (int i = 1; i <= 4; i++)
			{
			fseek(fileOut, 0x0, SEEK_CUR);
			fwrite(&values[0], sizeof(char), 0x1, fileOut);
			};
		
		// Placement
		pixelX += SCL;
		
		if (pixelX >= (length * SCL))
			{
			pixelX -= (length * SCL);
			pixelY -= SCL;
			};
		
		offStart += 0x8;
		offEnd += 0x8;
		};
	
	// Spheres.
	for (int i = 1; i <= pixels; i++)
		{
		// Unknown
		for (int j = 1; j <= 2; j++)
			{
			fseek(fileOut, 0x0, SEEK_CUR);
			fwrite(&values[0], sizeof(char), 0x1, fileOut);
			};
		
		// Color
		fseek(fileOut, 0x0, SEEK_CUR);
		fwrite(&colors[i], sizeof(char), 0x1, fileOut);
		fwrite(&values[0], sizeof(char), 0x1, fileOut);
		
		// Vertex
		fseek(fileOut, 0x0, SEEK_CUR);
		fwrite(&i, sizeof(short), 0x1, fileOut);
		
		// Size
		if (colors[i] != 0x0)
			{
			sphereSize = (SCL / 1.5f); // visible
			}
		else
			{
			sphereSize = 0x0; // invisible
			};
		
		fseek(fileOut, 0x0, SEEK_CUR);
		fwrite(&sphereSize, sizeof(short), 0x1, fileOut);
		
		offEnd += 0x8;
		};
	
	fseek(fileOut, 0x34, SEEK_SET);
	fwrite(&offStart, sizeof(short), 0x1, fileOut);
	
	fseek(fileOut, 0x3C, SEEK_SET);
	fwrite(&offStart, sizeof(short), 0x1, fileOut);
	
	fseek(fileOut, 0x44, SEEK_SET);
	fwrite(&offStart, sizeof(short), 0x1, fileOut);
	
	fseek(fileOut, 0x4C, SEEK_SET);
	fwrite(&offStart, sizeof(short), 0x1, fileOut);
	
	fseek(fileOut, 0x54, SEEK_SET);
	fwrite(&offStart, sizeof(short), 0x1, fileOut);
	
	fseek(fileOut, 0x5C, SEEK_SET);
	fwrite(&offEnd, sizeof(short), 0x1, fileOut);
	
	fclose(fileOut);
	
	printf("Model created successfully. \n");
	};

int main()
	{
	printf("Type in the name of the sprite: ");
	scanf("%s", fileName);
	
	loadSpriteCreateModel(fileName);
	
	return 0;
	};
