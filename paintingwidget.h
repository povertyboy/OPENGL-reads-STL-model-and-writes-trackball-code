#ifndef PAINTINGWIDGET_H
#define PAINTINGWIDGET_H
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QElapsedTimer>

#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>

class PaintingWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    PaintingWidget(QWidget *partent);
    ~PaintingWidget();

    GLfloat _angle;
    QVector3D _axisOfRotation;

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void keyPressEvent(QKeyEvent *keyEvent);
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    QVector3D getArcBallPosition(GLfloat x, GLfloat y);
    GLfloat angle(QVector3D startPosition, QVector3D endPosition);
    QVector3D axisOfRotation(QVector3D startPosition, QVector3D endPosition);

    void readStl();

private:
    QOpenGLBuffer *m_vbo, *m_cbo;
    QOpenGLVertexArrayObject *m_vao;
    QOpenGLShaderProgram *m_shader;
    QElapsedTimer *m_timer;
    float aspectRatio;
    GLfloat colorBuffer[900 * 3 *3];
    GLfloat vertexData[900 * 3 * 3];
    QVector3D camera_pos;
    QCursor cursor;//创建光标，保存默认光标形状

    QVector3D _startPosition;
    QVector3D _endPosition;
    bool mouseDownFlag;


};



#endif // PAINTINGWIDGET_H
