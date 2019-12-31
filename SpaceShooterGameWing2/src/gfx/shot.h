const int shotOffsets[] = {0, 28, };
const short shotPointOffsets[] = {0, 1, };
const unsigned short shotRes[][2] = {{4, 7}, };
const signed short shotPoints[][2] = {{2, 3}, };
const unsigned short shotPixels[] = {
0, 4096, 0, 0, 0, 45055, 45055, 4096, 7082, 57343, 61439, 6263, 0, 53247, 57343, 4096, 0, 45038, 45021, 0, 0, 25122, 21299, 0, 0, 5188, 1092, 0, };
Sprites shot(1, shotPixels, shotOffsets, shotRes, shotPoints, shotPointOffsets);
