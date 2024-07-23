#include "parcer.h"

void InitDataObject(object_t *obj) {
  obj->amount_vertex = 0;
  obj->number_vertex = 0;
  obj->number_face = 0;
  obj->amount_face = 0;
}

int FillDataFromFile(char *path, object_t *obj) {
  FILE *f = fopen(path, "r");
  int result = OK;

  if (f == NULL) {
    printf("Cannot open file %s\n", path);
    result = ERR_NO_FILE;
  } else {
    InitDataObject(obj);
    unsigned int amount_vertex = 3,  // итоговое количество vertex (x+y+z)
                                     // итоговое количество face
        num_str_v =
            1,  // количество строк vertex, сразу учитывается нулевая вершина
        num_str_f = 0;  // количество строк face
    int amount_face = 0;
    char buffer[10000];
    int count_number = 3;
    // считаем количество vertex & face для выделения памяти
    while ((fscanf(f, "%[^\n]", buffer)) != EOF) {
      if (num_str_v <= MAX_VERTEX + 1) {
        if (buffer[0] == 'v' && buffer[1] == ' ') {
          count_number = CountNumber(buffer);
          if (count_number != D3) {
            result = ERR_NO_3D;
            break;
          }
          num_str_v++;
          amount_vertex += count_number;
        }

        if (buffer[0] == 'f' && buffer[1] == ' ') {
          num_str_f++;
          count_number = CountNumber(buffer) * 2;
          amount_face += count_number;
        }
      } else {
        printf("MAX_VERTER < %d\n", amount_vertex);
        result = ERR_TOO_BIG;
        break;
      }
      fgetc(f);  // Reads in '\n' character and moves file
      buffer[0] = '\0';
    }

    if (result != OK)
      fclose(f);
    else {
      obj->amount_vertex = amount_vertex;
      obj->number_vertex = num_str_v;
      obj->value_vertex = (double *)calloc(amount_vertex, sizeof(double));
      obj->amount_face = amount_face;
      obj->value_face =
          (unsigned int *)calloc(amount_face, sizeof(unsigned int));
      obj->number_face = num_str_f;
      fseek(f, 0, SEEK_SET);
      //      f = fopen(path, "r");
      int indx_v = 0;
      int indx_f = 0;
      // int indx_str_face = 0;
      obj->value_vertex[indx_v++] = 0.0;
      obj->value_vertex[indx_v++] = 0.0;
      obj->value_vertex[indx_v++] = 0.0;
      while ((fscanf(f, "%[^\n]", buffer)) != EOF) {
        if (buffer[0] == 'v' && buffer[1] == ' ') {
          double f[D3];
          if (sscanf(buffer, "%*c %lf %lf %lf", &(f[0]), &(f[1]), &(f[2])) ==
              D3) {
            obj->value_vertex[indx_v++] = f[0];
            obj->value_vertex[indx_v++] = f[1];
            obj->value_vertex[indx_v++] = f[2];
          }  //  нужен ли else ???
        }

        if (buffer[0] == 'f' && buffer[1] == ' ') {
          unsigned int count = CountNumber(buffer);
          char *str = strtok(buffer, " ");
          unsigned int n = 0;
          int tmp, face1 = 0;
          int step = 0;
          do {
            str = strtok(NULL, " ");
            if (str) {
              n++;
              tmp = atoi(str);
              obj->value_face[indx_f++] = tmp;
              if (step == 0) {
                face1 = tmp;
              } else {
                obj->value_face[indx_f++] = tmp;
              }
              step++;
            }
          } while (str != NULL && n < count);
          obj->value_face[indx_f++] = face1;
        }
        fgetc(f);  // Reads in '\n' character and moves file
        buffer[0] = '\0';
      }
      fclose(f);
    }
  }
  return result;
}

void PrintObject(object_t *obj) {
  for (unsigned int i = 0; i < obj->amount_vertex; i++) {
    printf("%lf ", obj->value_vertex[i]);
    if ((i + 1) % 3 == 0) printf("\n");
  }
  printf("f \n");
  for (int i = 0; i < obj->amount_face; i++) {
    printf("%d ", obj->value_face[i]);
    if ((i + 1) % 6 == 0) printf("\n");
  }
}

void FreeObject(object_t *obj) {
  if (obj->number_vertex) {
    free(obj->value_vertex);
    obj->amount_vertex = 0;
    obj->number_vertex = 0;
    obj->value_vertex = NULL;
  }
  if (obj->number_face) {
    free(obj->value_face);
    obj->amount_face = 0;
    obj->number_face = 0;
    obj->value_face = NULL;
  }
}

/**
 *
 * @brief CountNumber
 * @param buffer
 * @example 'f 16/21/6 17/22/6 18/23/6 19/24/6 20/25/6 21/26/6 22/27/6 9/28/6
 * 8/29/6 11/30/6 13/31/6 14/32/6 23/33/6 24/34/6 25/35/6 26/36/6 27/37/6
 * 28/38/6 29/39/6 30/40/6 31/41/6 32/42/6 33/43/6 '
 * @return
 */
unsigned int CountNumber(char *buffer) {
  unsigned int n = 0;
  int is_prev_space = 0;

  for (int i = 0; buffer[i] != '\0' && buffer[i] != '\r'; i++) {
    if (buffer[i] == ' ' && !is_prev_space) {
      is_prev_space = 1;
    } else if (buffer[i] != ' ') {
      if (is_prev_space) {
        n++;
      }
      is_prev_space = 0;
    }
  }
  return n;
}

void Change_Scale(double *default_3d, double val, object_t *obj) {
  for (unsigned int i = 0; i < obj->amount_vertex; i++) {
    obj->value_vertex[i] = default_3d[i] * val / 100;
  }
}

void Move_Obj(object_t *obj, int val, int value_move) {
  for (unsigned int i = 0; i < obj->amount_vertex; i = i + 3) {
    double temp = val;
    obj->value_vertex[i + value_move] += temp / 100;
  }
}

void Rotation_X(object_t *obj, double value) {
  for (unsigned int i = 0; i < obj->amount_vertex; i = i + 3) {
    double tempY = obj->value_vertex[i + 1];
    double tempZ = obj->value_vertex[i + 2];
    obj->value_vertex[i + 1] =
        cos(value * M_PI / 180) * tempY - sin(value * M_PI / 180) * tempZ;
    obj->value_vertex[i + 2] =
        sin(value * M_PI / 180) * tempY + cos(value * M_PI / 180) * tempZ;
  }
}

void Rotation_Y(object_t *obj, double value) {
  for (unsigned int i = 0; i < obj->amount_vertex; i = i + 3) {
    double tempX = obj->value_vertex[i];
    double tempZ = obj->value_vertex[i + 2];
    obj->value_vertex[i] =
        cos(value * M_PI / 180) * tempX + sin(value * M_PI / 180) * tempZ;
    obj->value_vertex[i + 2] = (-1) * sin(value * M_PI / 180) * tempX +
                               cos(value * M_PI / 180) * tempZ;
  }
}

void Rotation_Z(object_t *obj, double value) {
  for (unsigned int i = 0; i < obj->amount_vertex; i = i + 3) {
    double tempX = obj->value_vertex[i];
    double tempY = obj->value_vertex[i + 1];
    obj->value_vertex[i] =
        cos(value * M_PI / 180) * tempX - sin(value * M_PI / 180) * tempY;
    obj->value_vertex[i + 1] =
        sin(value * M_PI / 180) * tempX + cos(value * M_PI / 180) * tempY;
  }
}
