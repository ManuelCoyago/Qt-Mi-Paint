#include "principal.h"
#include "ui_principal.h"

#define DEFAULT_ANCHO 3

Principal::Principal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Principal)
{
    ui->setupUi(this);
    // Instanciando la imagen (creando)
    mImagen = new QImage(this->size(),QImage::Format_ARGB32_Premultiplied);
    // Rellenar la imagen de color blanco
    mImagen->fill(Qt::white);
    // Instanciar el Painter a partir de la imagen
    mPainter = new QPainter(mImagen);
    mPainter->setRenderHint(QPainter::Antialiasing);
    // Inicializar otras variables
    mPuedeDibujar = false;
    mColor = Qt::black;
    mAncho = DEFAULT_ANCHO;
    mNumLineas = 0;
    mPuedeDibujarRectangulo = false;
    mPuedeDibujarLinea = false;
}

Principal::~Principal()
{
    delete ui;
    delete mPainter;
    delete mImagen;
}

void Principal::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);

    // Dibujar la imagen
    painter.drawImage(0, 0, *mImagen);

    if (mPuedeDibujarLinea)
    {
        // Crear un pincel y establecer atributos para el dibujo de líneas
        QPen pincel;
        pincel.setColor(mColor);
        pincel.setWidth(mAncho);
        painter.setPen(pincel);
        painter.drawLine(mInicialLinea, mFinalLinea);
    }
    else if (mPuedeDibujarRectangulo)
    {
        // Crear un pincel y establecer atributos para el dibujo de rectángulos
        QPen pincel;
        pincel.setColor(mColor);
        pincel.setWidth(mAncho);
        painter.setPen(pincel);
        painter.drawRect(mRectangulo);
    }

    event->accept();
}


void Principal::mousePressEvent(QMouseEvent *event) //
{
    if (mPuedeDibujarLinea)
        {
            // Iniciar el dibujo de líneas
            mPuedeDibujar = true;
            mInicialLinea = event->pos();
        }
        else if (mPuedeDibujarRectangulo)
        {
            // Iniciar el dibujo de rectángulos
            mRectangulo.setTopLeft(event->pos());
        }

        event->accept();
    }


void Principal::mouseMoveEvent(QMouseEvent *event)
{
 if(mPuedeDibujarLinea){
     // Validar si se puede dibujar
     if ( !mPuedeDibujar ) {
         // Acepta el evento
         event->accept();
         // Salir del método
         return;
     }
     // Capturar el punto a donde se mueve el mouse
     mFinal = event->pos();
     // Crear un pincel y establecer atributos
     QPen pincel;
     pincel.setColor(mColor);
     pincel.setWidth(mAncho);
     // Dibujar una linea
     mPainter->setPen(pincel);
     mPainter->drawLine(mInicial, mFinal);
     // Mostrar el número de líneas en la barra de estado
     ui->statusbar->showMessage("Número de líneas: " + QString::number(++mNumLineas));
     // Actualizar la interfaz (repinta con paintEvent)
     update();
     // actualizar el punto inicial
     mInicial = mFinal;
     event->accept();
 }else if (mPuedeDibujarRectangulo)
 {
     // Dibujar rectángulos
     mRectangulo.setBottomRight(event->pos());

     // Actualizar la interfaz (repinta con paintEvent)
     update();
 }

 event->accept();
}



void Principal::mouseReleaseEvent(QMouseEvent *event)
{
    if (mPuedeDibujarLinea)
    {
        // Finalizar el dibujo de líneas
        mPuedeDibujar = false;
    }
    else if (mPuedeDibujarRectangulo)
    {
        // Finalizar el dibujo de rectángulos
        mPuedeDibujarRectangulo = false;

        // Actualizar la interfaz (repinta con paintEvent)
        update();
    }

    event->accept();
}



void Principal::on_actionAncho_triggered()
{
    mAncho = QInputDialog::getInt(this,
                                  "Ancho del pincel",
                                  "Ingrese el ancho del pincel de dibujo",
                                  mAncho,
                                  1, 20);
}

void Principal::on_actionSalir_triggered()
{
    this->close();
}

void Principal::on_actionColor_triggered()
{
    mColor = QColorDialog::getColor(mColor,
                                    this,
                                    "Color del pincel");
}

void Principal::on_actionNuevo_triggered()
{
    mImagen->fill(Qt::white);
    mNumLineas = 0;
    update();
}

void Principal::on_actionGuardar_triggered()
{
    // Abrir cuadro de diálogo para obtener el nombre del archivo
    QString nombreArchivo = QFileDialog::getSaveFileName(this,
                                                         "Guardar imagen",
                                                         QString(),
                                                         "Imágenes .png (*.png)");
    // Validar que el nombre del archivo no sea vacío
    if ( !nombreArchivo.isEmpty() ){
        // Guardar imagen
        if (mImagen->save(nombreArchivo)){
            // Si todo va bien, muestra un mensaje de información
            QMessageBox::information(this,
                                     "Guardar imagen",
                                     "Archivo almacenado en: " + nombreArchivo);
        } else{
            // Si hay algún error, muestro advertencia
            QMessageBox::warning(this,
                                 "Guardar imagen",
                                 "No se pudo almacenar la imagen.");
        }
    }
}

void Principal::on_actionRect_nculos_triggered()
{
    mPuedeDibujarLinea = false;
    mPuedeDibujarRectangulo = true;
    //mPuedeDibujar = true; // Agregar esta línea para permitir dibujar el rectángulo
}

void Principal::on_actionLineas_triggered()
{
        mPuedeDibujarLinea = true;


}

void Principal::on_actionCircunferencias_triggered()
{

}



