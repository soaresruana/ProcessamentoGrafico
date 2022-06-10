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
#include <ctime>
#include <time.h>


using namespace cv;
using namespace std;
unsigned seed = time(0);

using std::filesystem::directory_iterator;

void drawText(Mat& image);

string carregaImagem();

string carregaImagemStickers();

void overlayImage(Mat* src, Mat* overlay, const Point& location);

void mouseCallback(int evt, int x, int y, int flags, void* param);

enum mouseEvents {NONE, LEFTBUTTON_DOWN};
int mouseEvent = LEFTBUTTON_DOWN;
int mousex, mousey;
Mat imgNariz, imgAlpha;
Mat imgOriginal, imgChange, sticker;

void menu();

//filtros


int main(int argc, char** argv)
{
      
    /*
    Criação das estruturas do OpenCV, mat4.
    imgOriginal = imagem original do diretório.
    imgChange = imagem alterado pelos filtros.

    Filtros:
    imgCinza, imgBlur, imgCanny, imgBlurCanny, imgDilate, imgErode
    
    Recortes/geral:
    imgCrop, imgReSize, imgScale;

    */
    

    Mat imgCinza, imgBlur, imgCanny, imgBlurCanny, imgDilate, imgErode;

    Mat imgCrop, imgReSize, imgScale;

    // Menu

    menu();
    /*Mat menu = imread("img/pikachu.jpg");
    namedWindow("Menu", WINDOW_AUTOSIZE);
    imshow("Menu", menu);*/
   
        
    //chamada da função para ler a imagem original
    imgOriginal = imread("img/" + carregaImagem());

    //Verifica se não tem erros ao ler a imagem
    if (!imgOriginal.data)
    {
        cout << "Erro ao carregar imagem" << endl;
        return -1;
    }

    

    // Criando uma janela    
    namedWindow("InstaCV", WINDOW_AUTOSIZE);

    //Ajustando o tamanho da janela
    resizeWindow("InstaCV", 1024, 573);
   

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

        // Redimencionando a imagem
        resize(imgChange, imgChange, Size(), 0.8, 0.8);

        //chamada da função para mostrar a imagem com as alterações aplicadas.
        imshow("InstaCV", imgChange);



        /*
        Funções de interação com o usuário via teclado
        27 = esc
        s = salvar imagem no diretório atual de imagens.

        */

        int iKey = (char)cv::waitKey(10);


        //ESC
        if (iKey == 27)
        {
            return -1;
        }

        /*A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,X,W,Y,Z*/

        //Blur
        if (iKey == 'B') {
         
            while (true) {
                GaussianBlur(imgOriginal, imgOriginal, Size(45, 45), 0, 0);
                break;
            }
            
        }

        // Canny
        if (iKey == 'C') {

            while (true) {
                Canny(imgOriginal, imgOriginal, 60, 60 * 3);
                break;
            }

        }

        //Escrever
        if (iKey == 'E') {
            while (true) {
                drawText(imgOriginal);
                break;
            }

        }

        // Conversão para Grayscale
        if (iKey == 'G') {
            while (true) {
                cvtColor(imgOriginal, imgOriginal, COLOR_BGR2GRAY);
                break;
            }

        }

        //Mouse CallBack
        if (iKey == 'M') {
            //cv::setMouseCallback("InstaCV", mouseEvent, &imgOriginal);
             //converte para 4 canais
            //

            //resize(imgNariz, imgNariz, Size(), 0.2, 0.2);


            //cvtColor(imgPikachu, imgPikachuAlpha, COLOR_BGR2BGRA);

            sticker = imread("stickers/narizinho.png", IMREAD_UNCHANGED);


            resize(sticker, sticker, Size(), 0.2, 0.2);
            cvtColor(imgOriginal, imgOriginal, COLOR_BGR2BGRA);



            setMouseCallback("InstaCV", mouseCallback, NULL);


        }

        // Coloca sticker com overlayImage - ajustar
        if (iKey == 'O') {

            Mat sticker2 = imread(carregaImagemStickers(), IMREAD_UNCHANGED);

            for (;;) {
                resize(sticker2, sticker2, Size(), 0.2, 0.2);

                cvtColor(imgOriginal, imgOriginal, COLOR_BGR2BGRA); //converte para 4 canais
                overlayImage(&imgOriginal, &sticker2, Point(170, 160));
            }
        }
        
        //Coloca sticker com coordenada via input
        if (iKey == 'R') {

            sticker = imread("stickers/narizinho.png");

            int x = 0;
            int y = 0;

            cout << "Digite a coordenada X do ponto: ";
            cin >> x;
            cout << "Digite a coordenada Y do ponto: ";
            cin >> y;


            for (;;) {

                // valor inicial, valorinicial + tam da imagem, matriz igual
                //erro encontrado: aviso libpng: iCCP: perfil sRGB incorreto conhecido
                //namedWindow("Sticker", WINDOW_AUTOSIZE);
                //imshow("Sticker", sticker);
                resize(sticker, sticker, Size(), 0.2, 0.2);
                //cvtColor(imgOriginal, imgOriginal, COLOR_BGR2BGRA);
                sticker.copyTo(imgOriginal.rowRange(x, x + sticker.cols).colRange(y, y + sticker.rows));
                break;
            }
        }
       
        // Salvar
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

        // Leitura de frame - video
        if (iKey == 'V') {


            VideoCapture cap("/Users/ruanabs/Desktop/Ruana/ProcessamentoGrafico/GB/ProcessamentoGrafico/instaCV/instaCV/video/resident7.mp4", cv::CAP_ANY);
            while (1) {
                double fps = cap.get(5);
                cout << "FRAMES PER SECOND USIN CAP.GET(cv::CAP_PROP_FPS) : " << fps << endl;

                int num_frame = cap.get(7);

                time_t start, end;

                Mat frame;

                cout << "capturing " << num_frame << " frames " << endl;

                time(&start);

                for (int i = 0; i < num_frame; i++) {
                    cap >> frame;
                    
                   
                }

                time(&end);

                double seconds = difftime(end, start);

                cout << "time taken :" << seconds << "seconds" << endl;

                fps = num_frame / seconds;
                cout << " estimado frames por second : " << fps << endl;

               
                while (cap.isOpened()) {

                    Mat frame;
                    cap.read(frame);
                    imshow("FRAME ", frame);

                    if (frame.empty()) {
                        break;
                    }
                    cap.release();


                }
                    
                                    
               
                

                



                // cap.read(frame);
                // cap.open("/Users/ruanabs/Desktop/Ruana/ProcessamentoGrafico/GB/ProcessamentoGrafico/instaCV/instaCV/video/resident7.mp4", cv::CAP_FFMPEG);
               
                

               


            }
 


            
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

Função definida para carregar uma imagem via console.
Variável path do tipo string contém o caminho do diretório de imagens, 
com iteração para listar o nome das imagens disponíveis. 

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

void overlayImage(Mat* src, Mat* overlay, const Point& location)
{
    for (int y = max(location.y, 0); y < src->rows; ++y)
    {
        int fY = y - location.y;
       
        if (fY >= overlay->rows)
            break;
        for (int x = max(location.x, 0); x < src->cols; ++x)
        {
            int fX = x - location.x;
            
            if (fX >= overlay->cols)
                break;
            double opacity = ((double)overlay->data[fY * overlay->step + fX * overlay->channels() + 3]) / 255;
            for (int c = 0; opacity > 0 && c < src->channels(); ++c)
            {
                unsigned char overlayPx = overlay->data[fY * overlay->step + fX * overlay->channels() + c];
                unsigned char srcPx = src->data[y * src->step + x * src->channels() + c];
                src->data[y * src->step + src->channels() * x + c] = srcPx * (1. - opacity) + overlayPx * opacity;
            }
        }
    }
}

void mouseCallback(int evt, int x, int y, int flags, void* param) {

    

    if (evt == EVENT_LBUTTONDOWN) { // Quando o botão da esquerda for clicado
        //sticker = imread("stickers/narizinho.png", IMREAD_UNCHANGED);
        //resize(sticker, sticker, Size(), 0.4, 0.4);

        cout << "Esquerda clicado, posicao X,Y:(" << x << "," << y << ")" << endl;
        mouseEvent = LEFTBUTTON_DOWN;

        mousex = x - sticker.cols / 2;
        //int difx = x - mousex; 
        //mousex = mousex + difx;

        printf("PONTO VALOR X:  %d", mousex);

        mousey = y - sticker.rows / 2;
        //int dify = y - mousey ; 
        //mousey = mousey + dify;

        printf("PONTO VALOR Y:  %d", mousey);

        //cout << "PONTO X" << mousex - sticker.cols / 2 << endl;
        //cout << "PONTO Y" << mousey - sticker.rows / 2 << endl;
        

        //cvtColor(imgOriginal, imgOriginal, COLOR_BGR2BGRA);
        //overlayImage(&imgOriginal, &sticker, Point(x - sticker.cols/2, y - sticker.rows/2));
        sticker.copyTo(imgOriginal.rowRange(x, x + sticker.cols).colRange(y, y + sticker.rows));
      
    }
    else {
        if (evt == EVENT_RBUTTONDOWN) { // Quando o botão da direita for clicado
        cout << "Direita clicado, posicao:(" << x << "," << y << ")" << endl;
        
        }
        else {
            if (evt == EVENT_MBUTTONDOWN) { // Quando o botão do meio for clicado
                cout << "Meio clicado, posicao:(" << x << "," << y << ")" << endl;
            }
            else {
                if (evt == EVENT_MOUSEMOVE) { // Quando o mouse mover
                    cout << "Movimentacao do mouse na posicao:(" << x << "," << y << ")" << endl;
                }
            
            }
        }
    
    }





}

/*

Função de Menu apenas para indicar os comandos disponíveis. 

*/

void menu() {

    printf("        *****MENU***** \n");
    printf("\n");
    printf("        ESC - fechar janela \n");
    printf("\n");
    printf("        B - Blur (GaussianBlur) \n");
    printf("\n");
    printf("        C - detector de bordas Canny \n");
    printf("\n");
    printf("        D - detector de bordas Sobel \n");
    printf("\n");
    printf("        E - escrever na imagem \n");
    printf("\n");
    printf("        F - Dilatacao \n");
    printf("\n");
    printf("        G - Grayscale \n");
    printf("\n");
    printf("        H - erosao \n");
    printf("\n");
    printf("        M - mouse callback \n");
    printf("\n");
    printf("        O - Sticker com OverlayImage \n");
    printf("\n");
    printf("        R - Sticker com Coordenada via input \n");
    printf("\n");
    printf("        S - Salvar imagem \n");
    printf("\n");
    printf("        1 - filtro instagram \n");
    printf("\n");
    printf("        2 - filtro instagram \n");
    printf("\n");
    printf("        3 - filtro instagram \n");
    printf("\n");
    printf("        4 - filtro instagram \n");
    printf("\n");
    printf("        5 - filtro instagram \n");
    printf("\n");
    printf("        6 - filtro instagram \n");
    printf("\n");
    printf("        7 - filtro instagram \n");
    printf("\n");
    printf("        8 - filtro instagram \n");
    printf("\n");
    printf("        9 - filtro instagram \n");
    printf("\n");
    printf("        10 - filtro instagram \n");
    printf("\n");
    


}

