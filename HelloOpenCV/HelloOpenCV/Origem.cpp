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

    //chama a função para ler a imagem
    imgOriginal = imread("person.jpg");

    /*
   Função Resize.
   Primeiro parâmetro recebe a imagem original.
   Segundo parâmetro recebe a imagem de destino.
   Terceiro as dimensões.
   */

    resize(imgOriginal, imgScale, Size(), 0.5, 0.5); // 0.4 0.4

    /*

    Filtros de imagens com intensidade, funcionam com uma média ponderada(olhar humano) sobre os 3 canais
    Por exemplo, para aplicar uma intensidade em cinza, usamamos esta média.

    - cvtColor, é uma função para alterar os canais de cores de uma imagem.
    O primeiro parâmetro é a imagem original. imgOriginal.
    O segundo parâmetro é a imagem de destino. imgCinza.
    O terceiro parâmetro é macros, que servem para realizar uma conversão de cores.

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

    Filtro para detecção de borda.
    
    Detalhe importante:
        O Canny por exemplo é um filtro para
        detecção de bordas e ele calcula tanto para x quanto para y,
        as diferenças de intensidades, no qual podemos parametrizar. 
        Então, quando tentamos extrair algo mais significativo de uma imagem, usamos junto ao 
        filtro de borda, o filtro de embassamento, para evitar a extração de "coisas poluidas das imagens." 

    */
   

    Canny(imgCinza, imgCanny, 25, 75);
    Canny(imgBlur, imgBlurCanny, 25, 75);


    /*
    Função Sobel
    O filtro Sobel é uma operação utilizada em processamento de imagem,
    aplicada sobretudo em algoritmos de detecção de contornos. 
    Em termos técnicos, consiste num operador que calcula diferenças finitas, 
    dando uma aproximação do gradiente da intensidade dos pixels da imagem.
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
    
    //chama a função para mostrar a imagem original
    //imshow("Pikachu Original",imgOriginal);

    // chama a imagem que recebeu aplicação da função cvtColor.
    imshow("Pikachu GRAYSCALE", imgCinza);

    // chama a imagem que recebeu aplicação da função Resize.
    imshow("Pikachu SCALE", imgScale);

    // chama a imagem que recebeu aplicação da função GaussianBlur (Embassado).
    imshow("Pikachu Embassado", imgBlur);

    // chama a imagem que recebeu aplicação da função GaussianBlur (Embassado).
    imshow("Pikachu CANNY", imgCanny);
    // chama a imagem que recebeu aplicação da função GaussianBlur (Embassado).
    imshow("Pikachu BLUR CANNY", imgBlurCanny);

    // chama a imagem que recebeu aplicação da função SOBEL
    imshow("Pikachu SOBEL", grad);


    

    //chama a função para esperar o usuario apertar uma tecla, para não fechar antes.
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