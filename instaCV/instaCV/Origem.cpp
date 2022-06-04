/*
Processamento Gr�fico
Autor: Ruana Boeira Soares
Nome Projeto: InstaCV
*/

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

#include <vector>
#include <string>
#include <filesystem>



#include <windows.h>

using namespace cv;
using namespace std;

using std::filesystem::directory_iterator;

void drawText(Mat& image);

int main(int argc, char** argv)
{
    //input via console
    string imgName;
    
    vector<string> listImg{"pikachu.jpg", "cat.jpg", "person.jpg"};

    for (size_t i = 0; i < listImg.size(); ++i) {
        cout << listImg[i] << "; ";
    }

    cout << endl;

    cout << "Digite o nome da imagem desejada: ";
    getline(cin, imgName);
    

        

    //cout << "Built with OpenCV " << CV_VERSION << endl;
    //Mat image;

    //Cria as estruturas do OpenCV, mat 4.
    Mat imgOriginal;
    Mat imgCinza, imgBlur, imgCanny, imgBlurCanny, imgDilate, imgErode;
    Mat imgCrop, imgReSize, imgScale;

    //chama a fun��o para ler a imagem
    imgOriginal = imread("img/"+imgName);

    /*
   Fun��o Resize.
   Primeiro par�metro recebe a imagem original.
   Segundo par�metro recebe a imagem de destino.
   Terceiro as dimens�es.
   */

    resize(imgOriginal, imgScale, Size(), 1.0, 1.0); // 0.4 0.4

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

    /*
    Equalizador de imagens, serve para equalizar as regi�es dos pixels.
    */

    Mat imgEqual;
    equalizeHist(imgCinza, imgEqual);


    /*

    Filtro de Dilata��o, busca uma forma de morfologia da estrutura��o que retorna uma mascara (kernel)
    A dilata��o aumenta as bordas, e a eros�o diminuiu.
    */

    Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
    dilate(imgBlurCanny, imgDilate, kernel);

    erode(imgDilate, imgErode, kernel);

    /*
    *
    Fun��o de recorte, pegar areas existentes na imagem.
    */

    // criando um retangulo nessas posi��es de pixel.
    Rect areaCrop(100, 100, 200, 200);

    // a partir do ponto 100x100 recorto 200 x 200, novaIMG img Base area do crop.

    imgCrop = imgOriginal(areaCrop);


    /*
    Fun��o para escrever na imagem
    DrawText, putText();
    */

    Mat img = imread("img/" + imgName);
    drawText(img);


    //CHAMADA DAS IMAGENS.

////chama a fun��o para mostrar a imagem original
//imshow("Pikachu Original",imgOriginal);

//// chama a imagem que recebeu aplica��o da fun��o cvtColor.
//imshow("Pikachu GRAYSCALE", imgCinza);

//// chama a imagem que recebeu aplica��o da fun��o RESIZE.
//imshow("Pikachu SCALE", imgScale);

//// chama a imagem que recebeu aplica��o da fun��o GaussianBlur (EMBASSADO).
//imshow("Pikachu Embassado", imgBlur);

//// chama a imagem que recebeu aplica��o da fun��o BORDA 1
//imshow("Pikachu CANNY", imgCanny);
//// chama a imagem que recebeu aplica��o da fun��o BORDA 2.
//imshow("Pikachu BLUR CANNY", imgBlurCanny);

//// chama a imagem que recebeu aplica��o da fun��o SOBEL
//imshow("Pikachu SOBEL", grad);

//// chama a imagem que recebeu aplica��o da fun��o EQUALIZADOR
//imshow("Pikachu EQUALIZADA", imgEqual);

//// chama a imagem que recebeu aplica��o da fun��o BLUR-KERNEL
//imshow("Pikachu BLUR KERNEL", imgBlurCanny);

//// chama a imagem que recebeu aplica��o da fun��o ERODIDO
//imshow("Pikachu ERODIDO", imgErode);

//// chama a imagem que recebeu aplica��o da fun��o DILATADA
//imshow("Pikachu DILATADO", imgDilate);


//// chama a imagem que recebeu aplica��o da fun��o de CORTE
//imshow("Pikachu RECORTE", imgCrop);


//// Chama a imagem com aplica��o de texto
//imshow("ESTE � UM PIKACHU", img);

/*

Fun��o que adiciona filtros em barras


*/
// Leitura da imagem original
    Mat src = imread("img/" + imgName);
    Mat dois = imread("img/" + imgName);
    Mat uniao;

    /*Uni�o de duas imagens na mesma janela.*/
    cv::hconcat(src, dois, uniao);



    //Verifica se n�o tem erros ao ler a imagem
    if (!src.data)
    {
        cout << "Erro ao carregar imagem" << endl;
        return -1;
    }

    // Criando mais de uma janela    
    namedWindow("Minha Window", WND_PROP_OPENGL);
    namedWindow("Minha Window 2", WINDOW_AUTOSIZE);

    //Criando uma track bar para mudar o brilho brightness
    int iSliderValue1 = 50;
    createTrackbar("Brilho", "Minha Window", &iSliderValue1, 100);

    //Criando track bar parar mudar o contraste
    int iSliderValue2 = 50;
    createTrackbar("Contraste", "Minha Window", &iSliderValue2, 100);

    // teste de janela

   

    while (true)

    {
        //Change the brightness and contrast of the image (For more infomation http://opencv-srf.blogspot.com/2013/07/change-contrast-of-image-or-video.html)

        Mat dst;
        int iBrightness = iSliderValue1 - 50;
        double dContrast = iSliderValue2 / 50.0;
        src.convertTo(dst, -1, dContrast, iBrightness);

        //show the brightness and contrast adjusted image
        imshow("Minha Window", dst);
        imshow("Minha Window 2", uniao);

        // testando janela

        //Mat load = LoadImageW();




        // Wait until user press some key for 50ms
       // int iKey = waitKey(50);
       int iKey = (char)cv::waitKey(10);

        //if user press 'ESC' key
        if (iKey == 27 || iKey == 's')
        {
            break;
        }

        if (iKey == 'a') {
            namedWindow("Minha Window 3", 3);
        }
    }



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



/*
    Fun��o Draw Text , escreve na imagem.
*/
void drawText(Mat& image)
{

    putText(image, "ESTE E UM PIKACHU",
        Point(20, 50),
        FONT_HERSHEY_COMPLEX, 1, // font face and scale
        Scalar(0, 0, 0), // white 255 255 255 black 0 0 0 
        1, LINE_AA); // line thickness and type
}

// bot�o