/*
Processamento Gráfico
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

    //chama a função para ler a imagem
    imgOriginal = imread("img/"+imgName);

    /*
   Função Resize.
   Primeiro parâmetro recebe a imagem original.
   Segundo parâmetro recebe a imagem de destino.
   Terceiro as dimensões.
   */

    resize(imgOriginal, imgScale, Size(), 1.0, 1.0); // 0.4 0.4

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

    /*
    Equalizador de imagens, serve para equalizar as regiões dos pixels.
    */

    Mat imgEqual;
    equalizeHist(imgCinza, imgEqual);


    /*

    Filtro de Dilatação, busca uma forma de morfologia da estruturação que retorna uma mascara (kernel)
    A dilatação aumenta as bordas, e a erosão diminuiu.
    */

    Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
    dilate(imgBlurCanny, imgDilate, kernel);

    erode(imgDilate, imgErode, kernel);

    /*
    *
    Função de recorte, pegar areas existentes na imagem.
    */

    // criando um retangulo nessas posições de pixel.
    Rect areaCrop(100, 100, 200, 200);

    // a partir do ponto 100x100 recorto 200 x 200, novaIMG img Base area do crop.

    imgCrop = imgOriginal(areaCrop);


    /*
    Função para escrever na imagem
    DrawText, putText();
    */

    Mat img = imread("img/" + imgName);
    drawText(img);


    //CHAMADA DAS IMAGENS.

////chama a função para mostrar a imagem original
//imshow("Pikachu Original",imgOriginal);

//// chama a imagem que recebeu aplicação da função cvtColor.
//imshow("Pikachu GRAYSCALE", imgCinza);

//// chama a imagem que recebeu aplicação da função RESIZE.
//imshow("Pikachu SCALE", imgScale);

//// chama a imagem que recebeu aplicação da função GaussianBlur (EMBASSADO).
//imshow("Pikachu Embassado", imgBlur);

//// chama a imagem que recebeu aplicação da função BORDA 1
//imshow("Pikachu CANNY", imgCanny);
//// chama a imagem que recebeu aplicação da função BORDA 2.
//imshow("Pikachu BLUR CANNY", imgBlurCanny);

//// chama a imagem que recebeu aplicação da função SOBEL
//imshow("Pikachu SOBEL", grad);

//// chama a imagem que recebeu aplicação da função EQUALIZADOR
//imshow("Pikachu EQUALIZADA", imgEqual);

//// chama a imagem que recebeu aplicação da função BLUR-KERNEL
//imshow("Pikachu BLUR KERNEL", imgBlurCanny);

//// chama a imagem que recebeu aplicação da função ERODIDO
//imshow("Pikachu ERODIDO", imgErode);

//// chama a imagem que recebeu aplicação da função DILATADA
//imshow("Pikachu DILATADO", imgDilate);


//// chama a imagem que recebeu aplicação da função de CORTE
//imshow("Pikachu RECORTE", imgCrop);


//// Chama a imagem com aplicação de texto
//imshow("ESTE É UM PIKACHU", img);

/*

Função que adiciona filtros em barras


*/
// Leitura da imagem original
    Mat src = imread("img/" + imgName);
    Mat dois = imread("img/" + imgName);
    Mat uniao;

    /*União de duas imagens na mesma janela.*/
    cv::hconcat(src, dois, uniao);



    //Verifica se não tem erros ao ler a imagem
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



/*
    Função Draw Text , escreve na imagem.
*/
void drawText(Mat& image)
{

    putText(image, "ESTE E UM PIKACHU",
        Point(20, 50),
        FONT_HERSHEY_COMPLEX, 1, // font face and scale
        Scalar(0, 0, 0), // white 255 255 255 black 0 0 0 
        1, LINE_AA); // line thickness and type
}

// botão