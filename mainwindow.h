#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <math.h>
#include <stdio.h>

#include <QColor>
#include <QColorDialog>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMainWindow>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QPalette>
#include <QSettings>
#include <QString>
#include <QTimer>
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

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  QString filename;  // путь до файла
  QColor bkgrColor;
  int check_color = 0;

  // save pics
  QGifImage *gif;
  QTimer *timer;

 private:
  Ui::MainWindow *ui;
  void FreeMemory();
  void SaveDefaultObj(object_t *obj);
  double *obj_default_vertex;
  int prev_pos_x, prev_pos_y, prev_pos_z;

 private slots:

  void on_toolButton_clear_clicked();
  void on_pushButton_3_clicked();
  void on_pushButton_4_clicked();
  void on_pushButton_color_background_2_clicked();
  void on_pushButton_color_of_lines_clicked();
  void on_pushButton_color_of_vertex_clicked();
  void on_doubleSpinBox_size_of_vertex_valueChanged(double arg1);
  void on_comboBox_type_of_vertex_activated(int index);
  void on_comboBox_type_line_activated(int index);
  void on_doubleSpinBox_11_valueChanged(double arg1);
  void on_comboBox_projection_activated(int index);
  void on_doubleSpinBox_mashtab_valueChanged(double arg1);
  void on_pushButton_save_clicked();
  void on_pushButton_save_gif_clicked();
  void RecordGif();
  void on_pushButton_save_settings_clicked();
  void on_pushButton_upload_settings_clicked();
  void on_pushButton_default_settings_clicked();
  void on_horizontalSlide_move_xyzr_valueChanged(int value);
  void on_spinBox__move_x_valueChanged(int arg1);
  void on_spinBox_move_y_valueChanged(int arg1);
  void on_spinBox_move_z_valueChanged(int arg1);

  void on_doubleSpinBox_8_valueChanged(double arg1);
  void on_doubleSpinBox_9_valueChanged(double arg1);
  void on_doubleSpinBox_10_valueChanged(double arg1);
  void on_pushButton_clear_screen_2_clicked();
  void on_pushButton__color_shape_clicked();
};
#endif  // MAINWINDOW_H
