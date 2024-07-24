int boatSize[]  = {2, 5, 6, 5, 2};

// makes sure ship placement doesn't wash previous placements
bool Collision(int (*boatPos)[3], bool * boatSet, int boat, int x, int y, bool hor)
{
    int potenCoords[boatSize[boat]][2];

    for (int i = 0; i < boatSize[boat]; i++) {
        if (hor) {
            potenCoords[i][0] = x + 2*i;
            potenCoords[i][1] = y; 
        }
        else {
            potenCoords[i][0] = x;
            potenCoords[i][1] = y + i; 
        }
    }

    for (int i = 0; i < 5; i++) {
        if (boatSet[i]) {
            for (int s = 0, takenX = boatPos[i][1], takenY = boatPos[i][2]; s < boatSize[i]; s++) {
                for (int p = 0; p < boatSize[boat]; p++) {
                    if (takenX == potenCoords[p][0] && takenY == potenCoords[p][1])
                        return true;
                }
                if (boatPos[i][0] == 0)
                    takenX += 2;
                else
                    takenY++;
            }
        }
    }
    return false;
}