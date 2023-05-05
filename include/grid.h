#ifndef _GRID_H
#define _GRID_H
struct Grid{
  int cellsPerSide;
  char* activeCells;
  struct Array *cellsMap,*borderMap;
  unsigned int* cellsElements;
  unsigned int cellsElementsCount;
  unsigned int changed;
  char (*cellUpdateProc)(struct Grid*,int,int);
};

struct Grid* Grid(int i);
void GridUpdate(struct Grid* grid);
void GridSetCell(struct Grid* grid,unsigned x,unsigned y,char value);
char GridGetCell(struct Grid* grid,unsigned x,unsigned y);
void GridSetCellsProc(struct Grid* grid,char (*cellProc)(struct Grid*,int,int));
void GridCallProc(struct Grid* grid);
void GridFree(struct Grid* grid);

#endif // !_GRID_H
