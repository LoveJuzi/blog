#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <vector>

#include <QOpenGLWindow>
#include <QOpenGLFunctions_3_3_Core>
#include <QTimer>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"
#include "square.h"
#include "squarecase.h"
#include "squarelightsource.h"

class MainWindow : public QOpenGLWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow() override;

public slots:
    void RotateTimeOutUpdate();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent *e) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void paintSquares();

private:
    QOpenGLFunctions_3_3_Core* _glcore;

    std::vector<std::shared_ptr<Square>> _squares;
    std::unique_ptr<Square> _square;
    std::unique_ptr<SquareCase> _squareCase;
    std::unique_ptr<SquareLightSource> _squareLightSource;

    Camera    _camera;
    QPoint    _center;
    glm::mat4 _projection;

    std::unique_ptr<QTimer>   _rotateTimer;
};
#endif // MAINWINDOW_H
