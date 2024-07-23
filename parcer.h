#ifndef _SRC_3DVIEWER_SRC_PARCER_H_
#define _SRC_3DVIEWER_SRC_PARCER_H_
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define D3 3
#define MAX_VERTEX 1000000

enum ERROR_3D {
  OK = 0,
  ERR_NO_FILE = 64,
  ERR_NO_3D = 65,
  ERR_TOO_BIG = 66,
};

typedef struct object {
  unsigned int amount_vertex;  // количество координат вершин (=count(x+y+z))
  unsigned int number_vertex;  // количество вершин
  unsigned int number_face;    // количество face
  int amount_face;             // итоговое количество face
  double *value_vertex;        // значение вершин (xyz)
  unsigned int *value_face;    // значение face
  // unsigned int *size_face;
} object_t;

void InitDataObject(object_t *obj);
int FillDataFromFile(char *path, object_t *obj);
void FreeObject(object_t *obj);
void PrintObject(object_t *obj);

unsigned int CountNumber(char *buffer);
void Resize(object_t *obj);

void Change_Scale(double *default_3d, double val, object_t *obj);
void Move_Obj(object_t *obj, int val, int value_move);
// rotation
void Rotation_X(object_t *obj, double value);
void Rotation_Y(object_t *obj, double value);
void Rotation_Z(object_t *obj, double value);

#endif  // _SRC_3DVIEWER_SRC_PARCER_H_
