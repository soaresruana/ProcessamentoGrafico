#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void drawText(Mat& image);

int main()
{
    cout << "Built with OpenCV " << CV_VERSION << endl;
    //Mat image;

    //Cria as estruturas do OpenCV, mat 4.
    Mat imgOriginal;
    Mat imgCinza, imgBlur, imgCanny, imgBlurCanny, imgDilate, imgEgrode;
    Mat imgCrop, imgReSize, imgScale;

    //chama a fun��o para ler a imagem
    imgOriginal = imread("person.jpg");

    /*
   Fun��o Resize.
   Primeiro par�metro recebe a imagem original.
   Segundo par�metro recebe a imagem de destino.
   Terceiro as dimens�es.
   */

    resize(imgOriginal, imgScale, Size(), 0.5, 0.5); // 0.4 0.4

    /*

    Filtros de imagens com intensidade, funcionam com uma m�dia ponderada(olhar humano) sobre os 3 canais
    Por exemplo, para aplicar uma intensidade em cinza, usamamos esta m�dia.

    - cvtColor, � uma fun��o para alterar os canais de cores de uma imagem.
    O primeiro par�metro � a imagem original. imgOriginal.
    O segundo par�metro � a imagem de destino. imgCinza.
    O terceiro par�metro � macros, que servem para realizar uma convers�o de cores.

     OpenCV denomina como BGR, os canais RGB.

    */
    cvtColor(imgScale, imgCinza, COLOR_BGR2GRAY);

    /*
    Filtro de Blur
    Size = tamanho da mascara.
    Size deve estar dentro no tamanho do resize. 
    */
    GaussianBlur(imgCinza, imgBlur, Size(5, 5), 5, 0);

    /*
    * 
    CANNY

    Filtro para detec��o de borda.
    
    Detalhe importante:
        O Canny por exemplo � um filtro para
        detec��o de bordas e ele calcula tanto para x quanto para y,
        as diferen�as de intensidades, no qual podemos parametrizar. 
        Ent�o, quando tentamos extrair algo mais significativo de uma imagem, usamos junto ao 
        filtro de borda, o filtro de embassamento, para evitar a extra��o de "coisas poluidas das imagens." 

    */
   

    Canny(imgCinza, imgCanny, 25, 75);
    Canny(imgBlur, imgBlurCanny, 25, 75);


    /*
    Fun��o Sobel
    O filtro Sobel � uma opera��o utilizada em processamento de imagem,
    aplicada sobretudo em algoritmos de detec��o de contornos. 
    Em termos t�cnicos, consiste num operador que calcula diferen�as finitas, 
    dando uma aproxima��o do gradiente da intensidade dos pixels da imagem.
    */

    Mat grad_x, grad_y;

    //16bits
    Sobel(imgCinza, grad_x, CV_16S, 1, 0, 1, 1, 0, BORDER_DEFAULT);
    Sobel(imgCinza, grad_y, CV_16S, 0, 1, 1, 1, 0, BORDER_DEFAULT);

    Mat abs_grad_x, abs_grad_y, grad;

    //Convertendo para o CV_8U 8bits

    convertScaleAbs(grad_x, abs_grad_x);
    convertScaleAbs(grad_y, abs_grad_y);

    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
    
    




                //CHAMADA DAS IMAGENS.
    
    //chama a fun��o para mostrar a imagem original
    //imshow("Pikachu Original",imgOriginal);

    // chama a imagem que recebeu aplica��o da fun��o cvtColor.
    imshow("Pikachu GRAYSCALE", imgCinza);

    // chama a imagem que recebeu aplica��o da fun��o Resize.
    imshow("Pikachu SCALE", imgScale);

    // chama a imagem que recebeu aplica��o da fun��o GaussianBlur (Embassado).
    imshow("Pikachu Embassado", imgBlur);

    // chama a imagem que recebeu aplica��o da fun��o GaussianBlur (Embassado).
    imshow("Pikachu CANNY", imgCanny);
    // chama a imagem que recebeu aplica��o da fun��o GaussianBlur (Embassado).
    imshow("Pikachu BLUR CANNY", imgBlurCanny);

    // chama a imagem que recebeu aplica��o da fun��o SOBEL
    imshow("Pikachu SOBEL", grad);


    

    //chama a fun��o para esperar o usuario apertar uma tecla, para n�o fechar antes.
    waitKey(0);

    







    /*
     Abertura da camera
    VideoCapture capture;
    capture.open(0);
    if (capture.isOpened())
    {
        cout << "Capture is opened" << endl;
        for (;;)
        {
            capture >> image;
            if (image.empty())
                break;
            drawText(image);
            imshow("Sample", image);
            if (waitKey(10) >= 0)
                break;
        }
    }
    else
    {
        cout << "No capture" << endl;
        image = Mat::zeros(480, 640, CV_8UC1);
        drawText(image);
        imshow("Sample", image);
        waitKey(0);
    }*/

    return 0;
}

void drawText(Mat& image)
{
    putText(image, "Hello OpenCV",
        Point(20, 50),
        FONT_HERSHEY_COMPLEX, 1, // font face and scale
        Scalar(255, 255, 255), // white
        1, LINE_AA); // line thickness and type
}