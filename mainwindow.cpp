#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  prev_pos_x = 0;
  prev_pos_y = 0;
  prev_pos_z = 0;
}

MainWindow::~MainWindow() {
  delete ui;
  FreeMemory();
}

void MainWindow::FreeMemory() {
  if (ui->openGLWidget->obj.number_vertex > 0) {
    delete[] obj_default_vertex;
  }
  FreeObject(&ui->openGLWidget->obj);
}

void MainWindow::SaveDefaultObj(object_t *obj) {
  obj_default_vertex = new double[obj->amount_vertex];
  memcpy(obj_default_vertex, obj->value_vertex,
         sizeof(double) * obj->amount_vertex);
}

void MainWindow::on_toolButton_clear_clicked() {
  ui->textEdit_obzor->clear();
  ui->textEdit_obzor->setText("Обзор...");
}

void MainWindow::on_pushButton_3_clicked() {
  filename = QFileDialog::getOpenFileName(this, "Выберите файл",
                                          QDir::homePath(), "*.obj");
  if (!filename.isEmpty()) ui->textEdit_obzor->setText(filename);
}

void MainWindow::on_pushButton_4_clicked() {
  if (!filename.isEmpty()) {
    char *filepath = filename.toLocal8Bit().data();
    object_t data_obj;
    int res = FillDataFromFile(filepath, &data_obj);
    qsizetype idx = filename.lastIndexOf('/') + 1;
    QString file_name = filename.right(filename.size() - idx);

    QString Edge = QString::number(data_obj.number_face);
    QString Vertexes = QString::number(data_obj.number_vertex - 1);

    if (res == OK) {
      SaveDefaultObj(&data_obj);
      ui->openGLWidget->obj = data_obj;
      ui->openGLWidget->update();
      ui->label_vertex->setText(Vertexes);
      ui->label_rebro->setText(Edge);
      ui->label_name->setText(file_name);
    } else if (res == ERR_TOO_BIG) {
      ui->textEdit_obzor->setText("Файл слишком большой");
    } else if (res == ERR_NO_FILE) {
      ui->textEdit_obzor->setText(filename + " не найден");
    } else if (res == ERR_NO_3D) {
      ui->textEdit_obzor->setText("Ошибочный формат");
    }
  } else {
    ui->textEdit_obzor->setText("Вы ничего не выбрали");
  }
}

void MainWindow::on_pushButton_color_background_2_clicked() {
  QColor colorLines = QColorDialog::getColor();
  ui->openGLWidget->bkgrColor =
      QVector4D(colorLines.redF(), colorLines.greenF(), colorLines.blueF(),
                colorLines.alphaF());
  ui->openGLWidget->update();
}

void MainWindow::on_pushButton_color_of_lines_clicked() {
  QColor colorLines = QColorDialog::getColor();
  ui->openGLWidget->line_x_color = colorLines.red();
  ui->openGLWidget->line_y_color = colorLines.green();
  ui->openGLWidget->line_z_color = colorLines.blue();
  ui->openGLWidget->update();
}

void MainWindow::on_pushButton_color_of_vertex_clicked() {
  QColor colorLines = QColorDialog::getColor();
  ui->openGLWidget->vert_x_color = colorLines.red();
  ui->openGLWidget->vert_y_color = colorLines.green();
  ui->openGLWidget->vert_z_color = colorLines.blue();
  ui->openGLWidget->update();
}

void MainWindow::on_doubleSpinBox_size_of_vertex_valueChanged(double arg1) {
  ui->openGLWidget->size_of_vertex = arg1;
  ui->openGLWidget->update();
}

void MainWindow::on_comboBox_type_of_vertex_activated(int index) {
  if (index == 0) {
    ui->openGLWidget->vertex_type = 0;
  } else if (index == 1) {
    ui->openGLWidget->vertex_type = 1;
  } else if (index == 2) {
    ui->openGLWidget->vertex_type = 2;
  }
  ui->openGLWidget->update();
}

void MainWindow::on_comboBox_type_line_activated(int index) {
  if (index == 0) {
    ui->openGLWidget->line_type = 0;
  } else if (index == 1) {
    ui->openGLWidget->line_type = 1;
  }
  ui->openGLWidget->update();
}

void MainWindow::on_doubleSpinBox_11_valueChanged(double arg1) {
  ui->openGLWidget->lineSize = arg1;
  ui->openGLWidget->update();
}

void MainWindow::on_comboBox_projection_activated(int index) {
  if (index == 0) {
    ui->openGLWidget->projection_type = 0;
  } else if (index == 1) {
    ui->openGLWidget->projection_type = 1;
  }
  ui->openGLWidget->update();
}

void MainWindow::on_doubleSpinBox_mashtab_valueChanged(double arg1) {
  Change_Scale(obj_default_vertex, arg1, &(ui->openGLWidget->obj));
  ui->openGLWidget->update();
}

void MainWindow::on_pushButton_save_clicked() {
  QString str = QFileDialog::getSaveFileName(0, "Сохранить изображение",
                                             "D:\\_Qt\\Qt\\images\\Image1",
                                             "*.png ;; *.jpg ;; *.bmp");
  if (!str.isEmpty()) {
    this->ui->openGLWidget->grab().save(str);
  }
}

void MainWindow::RecordGif() {
  gif->addFrame((ui->openGLWidget->grabFramebuffer()).scaled(640, 480));

  if (gif->frameCount() == 50) {  // (640x480, 10fps, 5s)
    gif->setDefaultDelay(100);
    timer->stop();
    QString str = QFileDialog::getSaveFileName(0, "Сохранить изображение",
                                               "~/gifka1", "*.gif");
    if (!str.isEmpty()) {
      gif->save(str);
    }
    ui->label_record->clear();
    delete gif;
  }
}

void MainWindow::on_pushButton_save_gif_clicked() {
  ui->label_record->setText("Идёт запись!!!");

  gif = new QGifImage();
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(RecordGif()));
  timer->start(100);
}

void MainWindow::on_pushButton_save_settings_clicked() {
  QSettings settings("settings_qt.ini", QSettings::IniFormat);

  settings.beginGroup("projection");
  settings.setValue("projectionType", ui->openGLWidget->projection_type);
  settings.endGroup();

  settings.beginGroup("backgroundColor");
  settings.setValue("backColorR", ui->openGLWidget->bkgrColor.x());
  settings.setValue("backColorG", ui->openGLWidget->bkgrColor.y());
  settings.setValue("backColorB", ui->openGLWidget->bkgrColor.z());
  settings.setValue("backColorA", ui->openGLWidget->bkgrColor.w());
  settings.endGroup();

  settings.beginGroup("vertexesColor");
  settings.setValue("vertexesColorX", ui->openGLWidget->vert_x_color);
  settings.setValue("vertexesColorY", ui->openGLWidget->vert_y_color);
  settings.setValue("vertexesColorZ", ui->openGLWidget->vert_z_color);
  settings.endGroup();

  settings.beginGroup("vertexesType");
  settings.setValue("pointType", ui->openGLWidget->vertex_type);
  settings.endGroup();

  settings.beginGroup("vertexesSize");
  settings.setValue("pointSize", ui->openGLWidget->size_of_vertex);
  settings.endGroup();

  settings.beginGroup("lineColor");
  settings.setValue("lineColorX", ui->openGLWidget->line_x_color);
  settings.setValue("lineColorY", ui->openGLWidget->line_y_color);
  settings.setValue("lineColorZ", ui->openGLWidget->line_z_color);
  settings.endGroup();

  settings.beginGroup("lineType");
  settings.setValue("lineTypeFlag", ui->openGLWidget->line_type);
  settings.endGroup();

  settings.beginGroup("lineSize");
  settings.setValue("lineSizes", ui->openGLWidget->lineSize);
  settings.endGroup();

  settings.beginGroup("mashtab");
  settings.setValue("valueMashtab", ui->openGLWidget->mashtab);
  settings.endGroup();

  settings.beginGroup("colorshape");
  settings.setValue("colorshape", ui->openGLWidget->color_shape);
  settings.endGroup();

  settings.beginGroup("path");
  char *filepath = filename.toLocal8Bit().data();
  settings.setValue("pathFile", filepath);
  settings.endGroup();
}

void MainWindow::on_pushButton_upload_settings_clicked() {
  QSettings settings("settings_qt.ini", QSettings::IniFormat);

  settings.beginGroup("projection");
  int typeProjection = settings.value("projectionType").toInt();
  settings.endGroup();
  ui->openGLWidget->projection_type = typeProjection;

  settings.beginGroup("backgroundColor");
  double color_r = settings.value("backColorR").toDouble();
  double color_g = settings.value("backColorG").toDouble();
  double color_b = settings.value("backColorB").toDouble();
  double color_a = settings.value("backColorA").toDouble();
  settings.endGroup();
  QVector4D colorBack = QVector4D(color_r, color_g, color_b, color_a);
  ui->openGLWidget->bkgrColor = colorBack;

  settings.beginGroup("vertexesColor");
  float color_x = settings.value("vertexesColorX").toFloat();
  float color_y = settings.value("vertexesColorY").toFloat();
  float color_z = settings.value("vertexesColorZ").toFloat();
  settings.endGroup();
  ui->openGLWidget->vert_x_color = color_x;
  ui->openGLWidget->vert_y_color = color_y;
  ui->openGLWidget->vert_z_color = color_z;

  settings.beginGroup("vertexesType");
  int type_point = settings.value("pointType").toInt();
  settings.endGroup();
  ui->openGLWidget->vertex_type = type_point;

  settings.beginGroup("vertexesSize");
  int size_point = settings.value("pointSize").toInt();
  settings.endGroup();
  ui->openGLWidget->size_of_vertex = size_point;

  settings.beginGroup("lineColor");
  float line_colorX = settings.value("lineColorX").toFloat();
  float line_colorY = settings.value("lineColorY").toFloat();
  float line_colorZ = settings.value("lineColorZ").toFloat();
  settings.endGroup();
  ui->openGLWidget->line_x_color = line_colorX;
  ui->openGLWidget->line_y_color = line_colorY;
  ui->openGLWidget->line_z_color = line_colorZ;

  settings.beginGroup("lineType");
  int line = settings.value("lineTypeFlag").toInt();
  settings.endGroup();
  ui->openGLWidget->line_type = line;

  settings.beginGroup("lineSize");
  float line_size = settings.value("lineSizes").toFloat();
  settings.endGroup();
  ui->openGLWidget->lineSize = line_size;

  settings.beginGroup("mashtab");
  float mashtab_value = settings.value("valueMashtab").toFloat();
  settings.endGroup();
  ui->openGLWidget->mashtab = mashtab_value;

  settings.beginGroup("colorshape");
  float colorshape = settings.value("colorshape").toFloat();
  settings.endGroup();
  ui->openGLWidget->color_shape = colorshape;
  if (colorshape == 0) {
    ui->pushButton__color_shape->setText("Раскрасить фигуру");
    check_color = 0;
  } else {
    ui->pushButton__color_shape->setText("Отменить цвет");
    check_color = 1;
  }

  ui->openGLWidget->update();
}

void MainWindow::on_pushButton_default_settings_clicked() {
  ui->comboBox_projection->setCurrentIndex(0);
  ui->comboBox_type_line->setCurrentIndex(0);
  ui->comboBox_type_of_vertex->setCurrentIndex(0);
  ui->doubleSpinBox_11->setValue(2.0);  // толщина ребер
  ui->doubleSpinBox_size_of_vertex->setValue(10.0);
  ui->doubleSpinBox_mashtab->setValue(100.0);
  ui->doubleSpinBox_8->setValue(0.0);
  ui->doubleSpinBox_9->setValue(0.0);
  ui->doubleSpinBox_10->setValue(0.0);
  ui->horizontalSlide_move_xyzr->setValue(0);

  check_color = 0;
  ui->openGLWidget->color_shape = 0;
  ui->pushButton__color_shape->setText("Раскрасить фигуру");

  ui->openGLWidget->vert_x_color = 255;
  ui->openGLWidget->vert_y_color = 255;
  ui->openGLWidget->vert_z_color = 255;
  ui->openGLWidget->line_x_color = 255;
  ui->openGLWidget->line_y_color = 255;
  ui->openGLWidget->line_z_color = 255;
  ui->openGLWidget->bkgrColor = QVector4D(0.0, 0.0, 0.0, 0.0);
  ui->openGLWidget->update();
  ui->openGLWidget->size_of_vertex = 10;
  ui->openGLWidget->lineSize = 1;
  ui->openGLWidget->line_type = 0;
  ui->openGLWidget->vertex_type = 0;
  ui->openGLWidget->projection_type = 0;
  ui->openGLWidget->update();
}

void MainWindow::on_horizontalSlide_move_xyzr_valueChanged(int value) {
  if (ui->radioButton_move_x->isChecked()) {
    Move_Obj(&(ui->openGLWidget->obj), value - prev_pos_x, 0);
    prev_pos_x = value;
  } else if (ui->radioButton_move_y->isChecked()) {
    Move_Obj(&(ui->openGLWidget->obj), value - prev_pos_y, 1);
    prev_pos_y = value;
  } else if (ui->radioButton_move_z->isChecked()) {
    Move_Obj(&(ui->openGLWidget->obj), value - prev_pos_z, 2);
    prev_pos_z = value;
  }
  ui->openGLWidget->update();
}

void MainWindow::on_spinBox__move_x_valueChanged(int arg1) {
  if (ui->radioButton_move_x->isChecked()) {
    Move_Obj(&(ui->openGLWidget->obj), arg1 - prev_pos_x, 0);
    prev_pos_x = arg1;
    ui->openGLWidget->update();
  }
}

void MainWindow::on_spinBox_move_y_valueChanged(int arg1) {
  if (ui->radioButton_move_y->isChecked()) {
    Move_Obj(&(ui->openGLWidget->obj), arg1 - prev_pos_y, 1);
    prev_pos_y = arg1;
    ui->openGLWidget->update();
  }
}

void MainWindow::on_spinBox_move_z_valueChanged(int arg1) {
  if (ui->radioButton_move_z->isChecked()) {
    Move_Obj(&(ui->openGLWidget->obj), arg1 - prev_pos_z, 2);
    prev_pos_z = arg1;
    ui->openGLWidget->update();
  }
}

void MainWindow::on_doubleSpinBox_8_valueChanged(double arg1) {
  Rotation_X(&(ui->openGLWidget->obj), arg1);
  ui->openGLWidget->update();
}

void MainWindow::on_doubleSpinBox_9_valueChanged(double arg1) {
  Rotation_Y(&(ui->openGLWidget->obj), arg1);
  ui->openGLWidget->update();
}

void MainWindow::on_doubleSpinBox_10_valueChanged(double arg1) {
  Rotation_Z(&(ui->openGLWidget->obj), arg1);
  ui->openGLWidget->update();
}

void MainWindow::on_pushButton_clear_screen_2_clicked() {
  FreeMemory();
  ui->openGLWidget->update();
}

void MainWindow::on_pushButton__color_shape_clicked() {
  if (check_color == 0) {
    check_color = 1;
    ui->openGLWidget->color_shape = 1;
    ui->pushButton__color_shape->setText("Отменить цвет");
    ui->openGLWidget->update();
  } else if (check_color == 1) {
    check_color = 0;
    ui->openGLWidget->color_shape = 0;
    ui->pushButton__color_shape->setText("Раскрасить фигуру");
    ui->openGLWidget->update();
  }
}
