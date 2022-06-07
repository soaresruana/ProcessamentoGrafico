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
#include <ctime>

using namespace cv;
using namespace std;
unsigned seed = time(0);

using std::filesystem::directory_iterator;

void drawText(Mat& image);
string carregaImagem();

string carregaImagemStickers();
void menu();

//filtros


int main(int argc, char** argv)
{
      
    /*
    Cria��o das estruturas do OpenCV, mat4.
    imgOriginal = imagem original do diret�rio.
    imgChange = imagem alterado pelos filtros.

    Filtros:
    imgCinza, imgBlur, imgCanny, imgBlurCanny, imgDilate, imgErode
    
    Recortes/geral:
    imgCrop, imgReSize, imgScale;

    */
    Mat imgOriginal, imgChange, sticker; 

    Mat imgCinza, imgBlur, imgCanny, imgBlurCanny, imgDilate, imgErode;

    Mat imgCrop, imgReSize, imgScale;

    // Menu

    menu();

    //chamada da fun��o para ler a imagem original
    imgOriginal = imread("img/" + carregaImagem());

    
    //Verifica se n�o tem erros ao ler a imagem
    if (!imgOriginal.data)
    {
        cout << "Erro ao carregar imagem" << endl;
        return -1;
    }

    // Criando uma janela    
    namedWindow("InstaCV", WINDOW_AUTOSIZE);
    

    //Criando uma track bar para mudar o brilho brightness
    int iSliderValue1 = 50;
    createTrackbar("Brilho", "InstaCV", &iSliderValue1, 100);

    //Criando track bar parar mudar o contraste
    int iSliderValue2 = 50;
    createTrackbar("Contraste", "InstaCV", &iSliderValue2, 100);

    //Criando track bar parar alterar o tamanho
    /*int iSliderValue2 = 50;
    createTrackbar("Resize", "InstaCV", &iSliderValue2, 100);*/

   


    while (true)

    {

        
        int iBrightness = iSliderValue1 - 50;
        double dContrast = iSliderValue2 / 50.0;
        imgOriginal.convertTo(imgChange, -1, dContrast, iBrightness);

        //chamada da fun��o para mostrar a imagem com as altera��es aplicadas.
        imshow("InstaCV", imgChange);

       
        /*
        Fun��es de intera��o com o usu�rio via teclado
        27 = esc
        s = salvar imagem no diret�rio atual de imagens.

        */

        int iKey = (char)cv::waitKey(10);
       
        
        //ESC
        if (iKey == 27)
        {
            return -1;
        }
        // SALVAR
        if (iKey == 'S') {
                       

            string newNameImg = "";

            cout << "Digite o nome da nova imagem para salvar: ";

            getline(cin, newNameImg);

            while(true) {
                bool isSalvo = imwrite("/Users/ruanabs/Desktop/Ruana/ProcessamentoGrafico/GB/ProcessamentoGrafico/instaCV/instaCV/img/salvo" + newNameImg + ".jpg", imgChange);
                if (isSalvo == false) {
                    cout << "erro ao salvar imagem" << endl;
                    cin.get(); //espera por um chave key
                    return -1;
                }
                cout << "Imagem salva com sucesso" << endl;

                namedWindow("Imagem Salva", WINDOW_AUTOSIZE);
                imshow("Imagem Salva", imgChange);
                               
                break;

            }

            
        }
        //ESCREVER
        if (iKey == 'E') {
            while (true) {
                drawText(imgOriginal);   
                break;
            }
                             
        }

        if (iKey == 'G') {
            while (true) {
                cvtColor(imgOriginal, imgOriginal, COLOR_BGR2GRAY);
                break;
            }

        }

        if (iKey == 'R') {
        
            // valor inicial, valorinicial + tam da imagem, matriz igual
            //erro encontrado: aviso libpng: iCCP: perfil sRGB incorreto conhecido
            sticker = imread("stickers/"+ carregaImagemStickers());
            //namedWindow("Sticker", WINDOW_AUTOSIZE);
            //imshow("Sticker", sticker);

            sticker.copyTo(imgOriginal.rowRange(150, 150 + 145).colRange(150, 150 + 145));
            
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
    string text;
    cout << "Digite o texto desejado: ";
    getline(cin, text);

    putText(image, text,
        Point(20, 50),
        FONT_HERSHEY_COMPLEX, 1, // font face and scale
        Scalar(0, 0, 0), // white 255 255 255 black 0 0 0 
        1, LINE_AA); // line thickness and type

    
}

/* 

Fun��o definida para carregar uma imagem via console.
Vari�vel path do tipo string cont�m o caminho do diret�rio de imagens, 
com itera��o para listar o nome das imagens dispon�veis. 

*/
string carregaImagem() {

    string imgName;
    
    string path = "/Users/ruanabs/Desktop/Ruana/ProcessamentoGrafico/GB/ProcessamentoGrafico/instaCV/instaCV/img";
    vector<string> listImg{};
    
    printf("Imagens disponiveis para carregar: \n");

    for (const auto& file : directory_iterator(path)) {
        printf("\n");
        cout << file.path() << endl;
    }
    cout << endl;

    cout << "Digite o nome da imagem desejada: ";
    getline(cin, imgName);

    return imgName;

}



string carregaImagemStickers() {

    string imgNameStiker;

    string path = "/Users/ruanabs/Desktop/Ruana/ProcessamentoGrafico/GB/ProcessamentoGrafico/instaCV/instaCV/stickers";
    vector<string> listImg{};

    printf("Stickers disponiveis para usar: \n");

    for (const auto& file : directory_iterator(path)) {
        printf("\n");
        cout << file.path() << endl;
    }
    cout << endl;

    cout << "Digite o nome do stiker desejado: ";
    getline(cin, imgNameStiker);

    return imgNameStiker;

}

/*

Fun��o de Menu apenas para indicar os comandos dispon�veis. 

*/

void menu() {

    printf("        *****MENU***** \n");
    printf("\n");
    printf("        ESC - fechar janela \n");
    printf("\n");
    printf("        S - salvar imagem \n");
    printf("\n");
    printf("        E - escrever na imagem \n");
    printf("\n");
    printf("        G - imagem cinza \n");
    printf("\n");
    


}

