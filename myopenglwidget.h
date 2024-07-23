#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QMouseEvent>
#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QWidget>
#include <QtGui>

#include "QtCore/qdatetime.h"
#include "qgifimage.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "parcer.h"
#ifdef __cplusplus
}
#endif

class MyOpenGLWidget : public QOpenGLWidget {
  Q_OBJECT
 public:
  MyOpenGLWidget(QWidget* parent = nullptr);
  virtual void mousePressEvent(QMouseEvent* pe) override;
  virtual void mouseMoveEvent(QMouseEvent* pe) override;
  void wheelEvent(QWheelEvent* pe) override;

  // for moving and scale
  double mashtab;
  double nSca, xPos, yPos, zPos;

  QVector4D bkgrColor = QVector4D(0.0, 0.0, 0.0, 1.0);
  int vertex = 0;

  // draw vertexes
  int vert_x_color = 255, vert_y_color = 255, vert_z_color = 255;
  float size_of_vertex = 10;
  int vertex_type = 0;
  void drawPoints(int x);

  // draw lines
  int line_x_color = 255, line_y_color = 255, line_z_color = 255;
  float lineSize = 1;
  int line_type = 0;

  object_t obj;

  // colorize
  int color_shape = 0;

  // projection
  int projection_type = 0;
  void projection(int x);

 protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int height) override;

 private:
  GLfloat m_xRotate;  // нужны для хранения углов поворота по оси X
  GLfloat m_yRotate;  // нужны для хранения углов поворота по оси Y
  GLfloat m_zRotate;
  GLfloat ratio;
  QPoint m_ptPosition;  // хранит координату указателя мыши в момент нажатия
};

#endif  // MYOPENGLWIDGET_H
