#include "myopenglwidget.h"

// glDrawElements(mode, count, type, indices);
// mode - как всегда выставляет режим типа полигонов,
// count - количество индексов, берущихся из массива,
// type - тип элементов в индексном массиве, может принимать значения:
// GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT или GL_UNSIGNED_INT. Обычно под индексы
// используют unsigned short. indices - указатель на массив с индексами.
// Заметьте, что этот указатель можно расценить, как указатель на элемент, с
// которого будет происходить расчёт объекта. То есть вы можете поставить
// указатель на любой элемент в массиве.

MyOpenGLWidget::MyOpenGLWidget(QWidget* parent) : QOpenGLWidget{parent} {
  InitDataObject(&obj);
  xPos = 0;
  yPos = 0;
  zPos = -4;
  nSca = 1;
}

void MyOpenGLWidget::initializeGL() { glEnable(GL_DEPTH_TEST); }

void MyOpenGLWidget::resizeGL(int width, int height) {
  glViewport(0, 0, width, height);
}

void MyOpenGLWidget::paintGL() {
  vertex = obj.number_vertex - 1;
  if (vertex > 0) {
    glClearColor(bkgrColor.x(), bkgrColor.y(), bkgrColor.z(), bkgrColor.w());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    projection(projection_type);
    glVertexPointer(3, GL_DOUBLE, 0, obj.value_vertex);
    if (color_shape == 1) {
      glColorPointer(3, GL_FLOAT, 0, obj.value_vertex);
      glEnableClientState(GL_COLOR_ARRAY);
    }

    glEnableClientState(GL_VERTEX_ARRAY);

    glPointSize(size_of_vertex);
    glColor3ub(vert_x_color, vert_y_color, vert_z_color);
    drawPoints(vertex_type);
    glLineWidth(lineSize);
    if (line_type == 1) {
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(5, 0x3333);
    }

    glColor3ub(line_x_color, line_y_color, line_z_color);

    glDrawElements(GL_LINES, obj.amount_face, GL_UNSIGNED_INT, obj.value_face);

    glDisableClientState(GL_VERTEX_ARRAY);
    if (color_shape == 1) {
      glDisableClientState(GL_COLOR_ARRAY);
    }

    if (line_type == 1) {
      glDisable(GL_LINE_STIPPLE);
    }
  }
}

void MyOpenGLWidget::drawPoints(int vertex_type) {
  vertex = obj.number_vertex - 1;
  if (vertex_type == 0) {
  } else if (vertex_type == 1) {
    glEnable(GL_POINT_SMOOTH);
    glDrawArrays(GL_POINTS, 1, vertex);
    glDisable(GL_POINT_SMOOTH);
  } else if (vertex_type == 2) {
    glDrawArrays(GL_POINTS, 1, vertex);
  }
}

void MyOpenGLWidget::projection(int projection_type) {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if (projection_type == 0) {
    nSca = 4;
    glFrustum(-1, 1, -1, 1, 1, qPow(300, 2));
  } else {
    if (nSca == 4) {
      nSca = 1;
    }
    ratio = (GLfloat)600 / (GLfloat)600;
    glOrtho(-2.0 / ratio, 2.0 / ratio, -2.0, 2.0, -50.0, 50.0);
  }
  glScalef(nSca, nSca, nSca);
  glTranslatef(xPos, 0, 0);
  glTranslatef(0, yPos, 0);
  glTranslatef(0, 0, zPos);
  glRotatef(m_xRotate, 1.0f, 0.0f, 0.0f);
  glRotatef(m_yRotate, 0.0f, 1.0f, 0.0f);
  glRotatef(m_zRotate, 0.0f, 0.0f, 1.0f);
}

void MyOpenGLWidget::mousePressEvent(QMouseEvent* pe) {
  m_ptPosition = pe->pos();
}

void MyOpenGLWidget::mouseMoveEvent(QMouseEvent* pe) {
  m_xRotate += 90 * (GLfloat)(pe->pos().x() - m_ptPosition.x()) / height();
  m_yRotate += 90 * (GLfloat)(pe->pos().y() - m_ptPosition.y()) / width();
  update();
  m_ptPosition = pe->pos();
}

void MyOpenGLWidget::wheelEvent(QWheelEvent* pe) {
  if (projection_type == 0) {
    int res = pe->angleDelta().y();
    if (res > 0)
      zPos += 0.1;
    else if (res < 0)
      zPos -= 0.1;
  } else {
    int res = pe->angleDelta().y();
    if (res > 0)
      nSca += 0.1;
    else if (res < 0 && nSca > 0.2)
      nSca -= 0.1;
  }
  update();
}
