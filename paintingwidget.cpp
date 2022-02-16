#include "paintingwidget.h"
#include <iostream>
#include <cmath>


static GLfloat array[27648*3*3];

static const char* VERTEX_SHADER_CODE =
        "#version 330 \n"
        "layout(location = 0) in vec3 posVertex;\n"
        "layout(location = 1) in vec3 colVertex;\n"
        "uniform mat4 MVP;\n"
        "out vec4 cols;\n"
        "void main() {\n"
        "  gl_Position = MVP * vec4(posVertex, 1.0f);\n"
        "  cols = vec4(colVertex, 1.0f);\n"
        "}\n";

static const char* FRAGMENT_SHADER_CODE =
        "#version 330\n"
        "in vec4 cols;\n"
        "out vec4 fragColor;\n"
        "void main() {\n"
        "  fragColor = cols;\n"
        "}\n";

PaintingWidget::PaintingWidget(QWidget* parent):
    QOpenGLWidget (parent), camera_pos(0.0f, 5.0f, 0.0f),
    _startPosition(0.0f, 0.0f, 0.0f), _endPosition(0.0f, 0.0f, 0.0f),
    m_vbo(nullptr), m_vao(nullptr), m_shader(nullptr), m_timer(nullptr){

    const GLfloat VERTEX_INIT_DATA[] = {
        //正方形 底面 Z=0
        -0.5f,-0.5f,0.0f,
        0.5f,-0.5f,0.0f,
        0.5f,0.5f,0.0f,

        -0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        //正方形 上面 Z=0.5
        -0.5f,-0.5f,0.5f,
        0.5f,-0.5f,0.5f,
        0.5f,0.5f,0.5f,

        -0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        //正方形 左面
        -0.5f,-0.5f,0.0f,
        -0.5f,-0.5f,0.5f,
        0.5f,0.5f,0.5f,

        0.5f,0.5f,0.0f,
        0.5f,-0.5f,0.5f,
        0.5f,0.5f,0.5f,

//        //face 1
//        -0.5, 0.0, -0.2887,
//        0.5, 0.0, -0.2887,
//        0.0, 0.0, 0.5774,
//        //face 2
//        -0.5, 0.0, -0.2887,
//        0.5, 0.0, -0.2887,
//        0.0, 0.8165, 0.0,
//        //face 3
//        -0.5, 0.0, -0.2887,
//        0.0, 0.0f, 0.5774,
//        0.0, 0.8165f, 0.0,
//        //face 4
//        0.5, 0.0, -0.2887,
//        0.0, 0.0, 0.5774,
//        0.0, 0.8165, 0.0,
//        -0.567711f,-0.440069f,0.323181f
//        -0.571085f,-0.424564f,0.319222f
//        -0.582243f,-0.440065f,0.324206f
//        -0.571085f,-0.424564f,0.319222f
//        -0.583216f,-0.427852f,0.318807f
//        -0.582243f,-0.440065f,0.324206f
//        -0.582243f,-0.440065f,0.324206f
//        -0.583216f,-0.427852f,0.318807f
//        -0.591019f,-0.438279f,0.319812f
//        -0.565891f,-0.456962f,0.324035f
//        -0.567711f,-0.440069f,0.323181f
//        -0.582015f,-0.456482f,0.326622f
//        array[0], array[1], array[2],
//        array[3], array[4], array[5],
//        array[6], array[7], array[8],

//        array[9], array[10], array[11],
//        array[12], array[13], array[14],
//        array[15], array[16], array[17],

//        array[18], array[18], array[20],
//        array[21], array[22], array[23],
//        array[24], array[25], array[26],

//        array[27], array[28], array[29],
//        array[30], array[31], array[32],
//        array[33], array[34], array[35],
//        -0.5f, -0.5f, -0.5f,
//        0.5f, -0.5f, -0.5f,
//        0.5f, 0.5f, -0.5f,
//        0.5f, 0.5f, -0.5f,
//        -0.5f, 0.5f, -0.5f,
//        -0.5f, -0.5f, -0.5f,

//        -0.5f, -0.5f, 0.5f,
//        0.5f, -0.5f, 0.5f,
//        0.5f, 0.5f, 0.5f,
//        0.5f, 0.5f, 0.5f,
//        -0.5f, 0.5f, 0.5f,
//        -0.5f, -0.5f, 0.5f,

//        -0.5f, 0.5f, 0.5f,
//        -0.5f, 0.5f, -0.5f,
//        -0.5f, -0.5f, -0.5f,
//        -0.5f, -0.5f, -0.5f,
//        -0.5f, -0.5f, 0.5f,
//        -0.5f, 0.5f, 0.5f,

//        0.5f, 0.5f, 0.5f,
//        0.5f, 0.5f, -0.5f,
//        0.5f, -0.5f, -0.5f,
//        0.5f, -0.5f, -0.5f,
//        0.5f, -0.5f, 0.5f,
//        0.5f, 0.5f, 0.5f,

//        -0.5f, -0.5f, -0.5f,
//        0.5f, -0.5f, -0.5f,
//        0.5f, -0.5f, 0.5f,
//        0.5f, -0.5f, 0.5f,
//        -0.5f, -0.5f, 0.5f,
//        -0.5f, -0.5f, -0.5f,

//        -0.5f, 0.5f, -0.5f,
//        0.5f, 0.5f, -0.5f,
//        0.5f, 0.5f, 0.5f,
//        0.5f, 0.5f, 0.5f,
//        -0.5f, 0.5f, 0.5f,
//        -0.5f, 0.5f, -0.5f,

       };
    const GLfloat COLOR_INIT_DATA[] = {
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,


       };
//    memcpy(this->vertexData, VERTEX_INIT_DATA, sizeof(this->vertexData));
//    std::cout << "不会把:"<<sizeof(this->vertexData)<<"sizeof(GLfloat):"<<
//                 sizeof(GLfloat)<<std::endl;
//    memcpy(this->colorBuffer, COLOR_INIT_DATA, sizeof(this->colorBuffer));
    memcpy(this->vertexData, array, sizeof(this->vertexData));
//    memcpy(this->colorBuffer, array, sizeof(this->colorBuffer));
    aspectRatio = (float) 800 / 600;
    m_timer = new QElapsedTimer;
    m_timer->start();
    setFocusPolicy(Qt::StrongFocus);

}
PaintingWidget::~PaintingWidget(){

}


void PaintingWidget::initializeGL()
{
    this->readStl();  // 先读取stl模型 获取顶点

    QOpenGLFunctions *f = this->context()->functions();
    f->glEnable(GL_DEPTH_TEST);
    m_shader = new QOpenGLShaderProgram();
    m_shader->addShaderFromSourceCode(QOpenGLShader::Vertex, VERTEX_SHADER_CODE);
    m_shader->addShaderFromSourceCode(QOpenGLShader::Fragment, FRAGMENT_SHADER_CODE);
    if (m_shader->link()) {
        qDebug("Shaders link success.");
    } else {
        qDebug("Shaders link failed!");
    }
    m_vao = new QOpenGLVertexArrayObject();
    m_vbo = new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);
    m_cbo = new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);
    m_vao->create();
    m_vao->bind();
    m_vbo->create();
    m_vbo->bind();
//    m_vbo->allocate(this->vertexData, sizeof(vertexData));
    m_vbo->allocate(array, sizeof(array)); // 放入顶点

    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,3*sizeof(GLfloat), 0);
    m_vbo->release();
    m_cbo->create();
    m_cbo->bind();
    m_cbo->allocate(this->colorBuffer, sizeof(colorBuffer));
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
    m_cbo->release();
    m_vao->release();

}

void PaintingWidget::paintGL()
{
    QOpenGLFunctions *f = this->context()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    f->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_vao->bind();
    m_shader->bind();
    QMatrix4x4 mvp;
    mvp.perspective(45.0f, this->aspectRatio, 0.1f, 100.0f);

    mvp.lookAt(camera_pos, \
               QVector3D(0.0f, 0.0f, 0.0f), \
               QVector3D(0.0f, 0.0f, -1.0f));


//    角度-旋转轴（PVM，物体本身的旋转、放缩等变换是编码在M矩阵中的，因此在计算MVP矩阵的地方，添加旋转代码）
    mvp.rotate(2*_angle, _axisOfRotation);
    m_shader->setUniformValue(m_shader->uniformLocation("MVP"), mvp);
    f->glDrawArrays(GL_TRIANGLES, 0, 27648 * 3);
    m_shader->release();
    m_vao->release();
    this->update();  // 这里不可调用repaint(),因为repaint()会调用paintGL()来完成绘图
}

void PaintingWidget::resizeGL(int w, int h)
{
    aspectRatio = (float)w / h;
}

void PaintingWidget::keyPressEvent(QKeyEvent *keyEvent){
    switch (keyEvent->key()){
        case Qt::Key_Right:
            camera_pos.setZ(camera_pos.z() + 0.1f);
            std::cout << "RIGHT" << std::endl;
            break;
        case Qt::Key_Left:
            camera_pos.setZ(camera_pos.z() - 0.1f);
            std::cout << "LEFT" << std::endl;
            break;
        case Qt::Key_Up:
            camera_pos.setX(camera_pos.x() + 0.1f);
            std::cout << "UP" << std::endl;
            break;
        case Qt::Key_Down:
            camera_pos.setX(camera_pos.x() - 0.1f);
            std::cout << "DOWN" << std::endl;
            break;
        case Qt::Key_Plus:
            camera_pos.setY(camera_pos.y() + 0.1f);
            std::cout << "PLUS" << std::endl;
            break;
        case Qt::Key_Minus:
            camera_pos.setY(camera_pos.y() - 0.1f);
            std::cout << "MINUS" << std::endl;
            break;
    }
    update();
}


void PaintingWidget::wheelEvent(QWheelEvent *event){
    if(event->delta()>0){//如果滚轮往上滚
        camera_pos.setY(camera_pos.y() - 0.1f);
        std::cout << "MOUSE_UP" << std::endl;

    }else{
        camera_pos.setY(camera_pos.y() + 0.1f);
        std::cout << "MOUSE_DOWN" << std::endl;

    }
    update();
}

void PaintingWidget::mousePressEvent(QMouseEvent *event){  // 鼠标按下
    if(event->buttons()==Qt::LeftButton){//如果鼠标按下的是左键

        //则改变鼠标形状为手掌，表示拖动状态。
        QCursor cursor1;//创建光标对象
        cursor1.setShape(Qt::OpenHandCursor);//设置光标形态
        setCursor(cursor1);//使用手掌光标
        GLfloat x = event->pos().x();
        GLfloat z = event->pos().y();
        std::cout << "MOUSE_PRESS_LEFT" <<x<<z<< std::endl;


        this->_startPosition = this->getArcBallPosition(x, z);
        this->mouseDownFlag = true;
    }
    update();
}
void PaintingWidget::mouseMoveEvent(QMouseEvent *event){  // 鼠标移动

    if (mouseDownFlag)
    {
        this->_endPosition = this->getArcBallPosition(event->pos().x(), event->pos().y());
        GLfloat cosAngle = angle(_startPosition, _endPosition);
        if (cosAngle > 1) { cosAngle = 1; }
        else if (cosAngle < -1) { cosAngle = -1; }
        std::cout << "cosAngle:" << cosAngle << std::endl;
        this->_angle = 10 * (GLfloat)(acos(cosAngle) / M_PI * 180);
        std::cout << "angle:" << this->_angle << std::endl;
        this->_axisOfRotation = axisOfRotation(_startPosition, _endPosition);

        std::cout << "MOUSE_MOVE" << std::endl;
    }

    update();
}

void PaintingWidget::mouseReleaseEvent(QMouseEvent *event){  // 鼠标松开
    //拖动完成后，光标恢复默认形状
    setCursor(cursor);
    //或者直接用自带恢复鼠标指针形状的函数为：QApplication::restoreOverrideCursor();
    //但是需要前面设置哪个是默认的光标形状，用这个函数setOverrideCursor()函数

    std::cout << "MOUSE_RELEASE_LEFT" << std::endl;

    this->mouseDownFlag = false;

}

QVector3D PaintingWidget::getArcBallPosition(GLfloat x, GLfloat y)
{
    GLfloat rx = (2*x/800) - 1;
    GLfloat ry = -((2*y/600) - 1);
    GLfloat zz = 1 - rx*rx - ry*ry;
    GLfloat rz = ( zz>0?sqrt(zz):0);
    QVector3D result(rx, ry, rz);
    std::cout << "X:" << result.x() <<"Y:" <<result.y() <<"Z:" <<result.z() <<std::endl;

   return result;
}

GLfloat PaintingWidget::angle(QVector3D startPosition, QVector3D endPosition)
{
    GLfloat delta = (startPosition.x()*endPosition.x() +
                     startPosition.y()*endPosition.y() +
                     startPosition.z()*endPosition.z()) /
            (sqrt((startPosition.x()*startPosition.x() +
                  startPosition.y()*startPosition.y() +
                  startPosition.z()*startPosition.z()) *
                 (endPosition.x()*endPosition.x() +
                  endPosition.y()*endPosition.y() +
                  endPosition.z()*endPosition.z() )) );
    std::cout << "delta:" << delta <<std::endl;
    return delta;  // acos(delta);
}

QVector3D PaintingWidget::axisOfRotation(QVector3D startPosition, QVector3D endPosition)
{
    return QVector3D(startPosition.y()*endPosition.z()-startPosition.z()*endPosition.y(),
                     -(startPosition.x()*endPosition.z()-startPosition.z()*endPosition.x()),
                     startPosition.x()*endPosition.y()-startPosition.y()*endPosition.x() );
}


void PaintingWidget::readStl()
{

    GLint fileSize;  //记录文件大小
    QFile STL_file("bear.stl");
    bool isOk = STL_file.open(QIODevice::ReadOnly);
    fileSize = STL_file.size();
    GLchar * buf = (char *)malloc(sizeof(char)*fileSize);
    std::cout << "FILESIZE:" << fileSize << std::endl;

    if(isOk == true)
    {
       std::cout << "打开成功" << std::endl;
       QDataStream stream(&STL_file);
       stream.readRawData(buf,fileSize);
       STL_file.close();
    }

    const GLchar* p = buf;
    GLchar name[80];
    GLfloat temp;
    GLint binary_i;
    GLint triangle_num;
    memcpy(name,p,80);//记录文件名
    std::cout << "NAME:" << name << std::endl;

    p += 80;  //跳过文件名
    memcpy(&triangle_num,p,4);  //记录三角形个数
    p += 4; //跳过个数标识
    qDebug() << "triangle_num:" << triangle_num<<"\n"; //控制台打印三角形个数
    for (binary_i=0;binary_i<triangle_num;binary_i++)
    {
         p += 12;//跳过头部的法向量
         memcpy(&temp,p,4);p+=4;array[binary_i*9+0] = GLfloat(temp);
         memcpy(&temp,p,4);p+=4;array[binary_i*9+1] = GLfloat(temp);
         memcpy(&temp,p,4);p+=4;array[binary_i*9+2] = GLfloat(temp);

         memcpy(&temp,p,4);p+=4;array[binary_i*9+3] = GLfloat(temp);
         memcpy(&temp,p,4);p+=4;array[binary_i*9+4] = GLfloat(temp);
         memcpy(&temp,p,4);p+=4;array[binary_i*9+5] = GLfloat(temp);

         memcpy(&temp,p,4);p+=4;array[binary_i*9+6] = GLfloat(temp);
         memcpy(&temp,p,4);p+=4;array[binary_i*9+7] = GLfloat(temp);
         memcpy(&temp,p,4);p+=4;array[binary_i*9+8] = GLfloat(temp);
//         std::cout << "check" <<
//         array[binary_i*9+0] << array[binary_i*9+1] << array[binary_i*9+2] <<
//         array[binary_i*9+3] << array[binary_i*9+4] << array[binary_i*9+5] <<
//         array[binary_i*9+6] << array[binary_i*9+7] << array[binary_i*9+8] <<
//         std::endl;
//         //vertex
//         memcpy(&temp,p,4);vertices[VerticesCnt] = temp ;p+=4;VerticesCnt++;//vertices[0]赋值
//         memcpy(&temp,p,4);vertices[VerticesCnt] = temp;p+=4;VerticesCnt++;//vertices[1]赋值
//         memcpy(&temp,p,4);vertices[VerticesCnt] = temp ;p+=4;VerticesCnt++;//vertices[2]赋值
//         vertices[VerticesCnt] = 1.0;  VerticesCnt++;
//         vertices[VerticesCnt] = 0.9;  VerticesCnt++;
//         vertices[VerticesCnt] = 0.6;  VerticesCnt++;
//        //vertex
//         memcpy(&temp,p,4);  vertices[VerticesCnt] = temp ;p+=4;VerticesCnt++;//vertices[0]赋值
//         memcpy(&temp,p,4);  vertices[VerticesCnt] = temp ;p+=4;VerticesCnt++;//vertices[1]赋值
//         memcpy(&temp,p,4);  vertices[VerticesCnt] = temp ;p+=4;VerticesCnt++;//vertices[2]赋值
//         vertices[VerticesCnt] = 1.0;  VerticesCnt++;
//         vertices[VerticesCnt] = 0.9;  VerticesCnt++;
//         vertices[VerticesCnt] = 0.6;  VerticesCnt++;
//         //vertex
//         memcpy(&temp,p,4);vertices[VerticesCnt] = temp;p+=4;VerticesCnt++;//vertices[0]赋值
//         memcpy(&temp,p,4);vertices[VerticesCnt] = temp ;p+=4;VerticesCnt++;//vertices[1]赋值
//         memcpy(&temp,p,4);vertices[VerticesCnt] = temp ;p+=4;VerticesCnt++;//vertices[2]赋值
//         vertices[VerticesCnt] = 1.0;VerticesCnt++;
//         vertices[VerticesCnt] = 0.9;VerticesCnt++;
//         vertices[VerticesCnt] = 0.6;VerticesCnt++;
         p += 2;//跳过尾部标志
     }
    qDebug() << "大小：" << (sizeof(array) / sizeof(array[0])) << "\n";
    free(buf);
}
