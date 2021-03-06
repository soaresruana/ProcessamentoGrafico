/*

Processamento Gr?fico
Autor: Ruana Boeira Soares
Processamento Gr?fico.

Nome Projeto: InstaCV
13/06/2022

*/

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/photo.hpp>
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

void interpolation(uchar* lut, float* curve, float* originalValue);

void on_trackbar(int, void*);
int g_slider; //slider video
int g_slider_max; //slider video



enum mouseEvents {NONE, LEFTBUTTON_DOWN};
int mouseEvent = LEFTBUTTON_DOWN;
int mousex, mousey;
Mat imgOriginal, imgCinza, imgEdge, imgChange, sticker;

void menu();



int main(int argc, char** argv)
{
    

    // Menu

    menu();
     
        
    //chamada da fun??o para ler a imagem original
    imgOriginal = imread("img/" + carregaImagem());

    //Verifica se n?o tem erros ao ler a imagem
    if (!imgOriginal.data)
    {
        cout << "Erro ao carregar imagem" << endl;
        return -1;
    }
    //Ajusta o tamanho da imagem original
    resize(imgOriginal, imgOriginal, Size(1024, 700), 1.0, 1.0);

    // Criando uma janela    
    namedWindow("InstaCV", WINDOW_AUTOSIZE);

    //Ajustando o tamanho da janela
    resizeWindow("InstaCV", 800, 573);
   

    //Criando uma track bar para mudar o brilho brightness
    int iSliderValue1 = 50;
    createTrackbar("Brilho", "InstaCV", &iSliderValue1, 100);

    //Criando track bar parar mudar o contraste
    int iSliderValue2 = 50;
    createTrackbar("Contraste", "InstaCV", &iSliderValue2, 100);

    

    while (true)

    {

        int iBrightness = iSliderValue1 - 50;
        double dContrast = iSliderValue2 / 50.0;
        imgOriginal.convertTo(imgChange, -1, dContrast, iBrightness);

       

        // Redimencionando a imagem
        resize(imgChange, imgChange, Size(), 0.8, 0.8);

        //chamada da fun??o para mostrar a imagem com as altera??es aplicadas.
        imshow("InstaCV", imgChange);



        /*
        Fun??es de intera??o com o usu?rio via teclado
        */

        int iKey = (char)cv::waitKey(10);


        //ESC
        if (iKey == 27)
        {
            return -1;
        }

       

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
                cvtColor(imgOriginal, imgOriginal, COLOR_BGR2GRAY);
                Canny(imgOriginal, imgOriginal, 50, 130);
                break;
            }

        }

        //Sobel
        if (iKey == 'D') {

            while (true) {

                Mat grad_x, grad_y;

                //16bits
                Sobel(imgOriginal, grad_x, CV_16S, 1, 0, 1, 1, 0, BORDER_DEFAULT);
                Sobel(imgOriginal, grad_y, CV_16S, 0, 1, 1, 1, 0, BORDER_DEFAULT);

                Mat abs_grad_x, abs_grad_y;

                //Convertendo para o CV_8U 8bits

                convertScaleAbs(grad_x, abs_grad_x);
                convertScaleAbs(grad_y, abs_grad_y);

                addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, imgOriginal);

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

        //Dilatacao
        if (iKey == 'F') {
            while (true) {

                Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
                dilate(imgOriginal, imgOriginal, kernel);

                break;
            }

        }

        // Convers?o para Grayscale
        if (iKey == 'G') {
            while (true) {
                cvtColor(imgOriginal, imgOriginal, COLOR_BGR2GRAY);
                break;
            }

        }

        //Erosao
        if (iKey == 'H') {
            while (true) {
                Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
                erode(imgOriginal, imgOriginal, kernel);
                break;
            }
        }

        //Mouse CallBack - Sticker - charuto
        if (iKey == 'I') {

            sticker = imread("stickers/charuto.png", IMREAD_UNCHANGED);

            resize(sticker, sticker, Size(), 0.1, 0.1);
            cvtColor(imgOriginal, imgOriginal, COLOR_BGR2BGRA);

            setMouseCallback("InstaCV", mouseCallback, NULL);


        }

        //Mouse CallBack - Sticker - oculos
        if (iKey == 'J') {

            sticker = imread("stickers/oculos.png", IMREAD_UNCHANGED);

            resize(sticker, sticker, Size(), 0.4, 0.4);
            cvtColor(imgOriginal, imgOriginal, COLOR_BGR2BGRA);

            setMouseCallback("InstaCV", mouseCallback, NULL);


        }

        //Mouse CallBack - Sticker Nariz
        if (iKey == 'M') {
            
            sticker = imread("stickers/narizinho.png", IMREAD_UNCHANGED);

            resize(sticker, sticker, Size(), 0.2, 0.2);
            cvtColor(imgOriginal, imgOriginal, COLOR_BGR2BGRA);

            setMouseCallback("InstaCV", mouseCallback, NULL);


        }

        // Coloca sticker com overlayImage - coordenadas via teclado
        if (iKey == 'O') {

            Mat sticker2 = imread("stickers/" + carregaImagemStickers(), IMREAD_UNCHANGED);

            
                resize(sticker2, sticker2, Size(), 0.2, 0.2);

                cvtColor(imgOriginal, imgOriginal, COLOR_BGR2BGRA); //converte para 4 canais
                   
                    int x, y;

                    cout << "Valor da coordenada x: ";
                    cin >> x;
                    cout << "valor da coordenada y: ";
                    cin >> y;

                overlayImage(&imgOriginal, &sticker2, Point(x, y));
                
        }
        
        //Coloca sticker com coordenada via input - NARIZ
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
       
        // Salvar Imagem
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

        // Execucao de video manipulacao do trackbar
        if (iKey == 'V') {

            VideoCapture videoCapture("/Users/ruanabs/Desktop/Ruana/ProcessamentoGrafico/GB/ProcessamentoGrafico/instaCV/instaCV/video/resident7.mp4");
            Mat edges;
            
            if (!videoCapture.isOpened()) {
                cout << "Erro ao carregar o video" << endl;
                return -1;
            }

            //set
            g_slider = 0;
            g_slider_max = 255;
            
            namedWindow("Whatching Resident Evil 7", WINDOW_NORMAL);
            createTrackbar("Filter", "Whatching Resident Evil 7", &g_slider, g_slider_max, on_trackbar);

            

            
            while (1) {

                Mat frame;

                videoCapture >> frame;

                cvtColor(frame, edges, COLOR_BGR2GRAY);
                GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
                Canny(edges, edges, g_slider, 30, 3);
                imshow("edges", edges);

                

                if (frame.empty()) {
                    break;
                }

               
                imshow("Whatching Resident Evil 7", frame);

                //ESC parar video
                char c = (char)waitKey(25);
                if (c == 27) {
                    break;
                }
            }

            
            videoCapture.release();
 


            
        }

        // Salvando um Frame
        if (iKey == 'X') {
            

            VideoCapture videoCapture("/Users/ruanabs/Desktop/Ruana/ProcessamentoGrafico/GB/ProcessamentoGrafico/instaCV/instaCV/video/resident7.mp4");
           

            if (!videoCapture.isOpened()) {
                cout << "Erro ao carregar o video" << endl;
                return -1;
            }

            int frame_width = int(videoCapture.get(3));
            int frame_height = int(videoCapture.get(4));

            int size = (frame_width, frame_height);

            string text;

            cout << "Digite o nome para salvar: ";
            cin >> text;

            VideoWriter video("/Users/ruanabs/Desktop/Ruana/ProcessamentoGrafico/GB/ProcessamentoGrafico/instaCV/instaCV/video/"+ text + ".avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, Size(frame_width, frame_height));
            
           
            while (true) {

                Mat frame;

                videoCapture >> frame;

                               
                if (frame.empty()) {
                    break;
                }

                video.write(frame);


            }


        }

        //Uniao de duas imagens
        if (iKey == 'W') {

            // Leitura da imagem original
            //"img/" + carregaImagem()
            Mat src = imread("img/" + carregaImagem());
            Mat dois = imread("img/" + carregaImagem());
            resize(src, src, Size(400,400), 0.5, 0.5);
            resize(dois, dois, Size(400,400), 0.5, 0.5);
            Mat uniao;

            /*Uni?o de duas imagens na mesma janela.*/
            cv::hconcat(src, dois, uniao);
            imgOriginal = uniao;
            
        }

      
        
        /*Filtros integram*/

        //Retira o ruido da imagem
        if (iKey == '0') {

            Mat output;

            int kernelSize = 5;

            Mat kernel = Mat::ones(kernelSize, kernelSize, CV_32F);
            kernel = kernel / (float)(kernelSize * kernelSize);

            filter2D(imgOriginal, output, -1, kernel, Point(-1, -1), 0, BORDER_DEFAULT);

            imgOriginal = output;


        }

        //Cartoon
        if (iKey == '1') {

           
            //Converte para cinza
            cvtColor(imgOriginal, imgCinza, COLOR_BGR2GRAY);
            

            ////aplica blur
            GaussianBlur(imgCinza, imgCinza, Size(5, 5), 0);

            Laplacian(imgCinza, imgEdge, -1, 5);
            
            convertScaleAbs(imgEdge, imgEdge);

            //inverte a imagem
            imgEdge = 255 - imgEdge;

            threshold(imgEdge, imgEdge, 150, 255, THRESH_BINARY);

            Mat edgePreservingImage;
            edgePreservingFilter(imgOriginal, edgePreservingImage, 2, 50, 0.4);

            //output;
            imgOriginal = Scalar::all(0);

            cv::bitwise_and(edgePreservingImage, edgePreservingImage, imgOriginal, imgEdge);
            


        }

        //Pencil
        if (iKey == '2') {
        
            cvtColor(imgOriginal, imgCinza, COLOR_BGR2GRAY);

            Mat grayImageInv = 255 - imgCinza;

            GaussianBlur(grayImageInv, grayImageInv, Size(21, 21), 0);

            divide(imgCinza, 255 - grayImageInv, imgOriginal, 256.0);
        
        
        }

        //Paraiso artificial
        if (iKey == '3') {

            //cria uma c?pia da imagem original
            Mat output = imgOriginal.clone();

            // Calcula o tamanho do kernel e pega os efeitos, normaliza

            float vignetteScale = 6;
            
            int k = min(output.rows, output.cols) / vignetteScale;
            
            Mat kernelX = getGaussianKernel(output.cols, k);
            Mat kernelY = getGaussianKernel(output.rows, k);
            Mat kernelx_transpose;
            transpose(kernelX, kernelx_transpose);
            Mat kernel = kernelY * kernelx_transpose;
            //cout << kernel.size() << endl;

            Mat mask;
            normalize(kernel, mask, 0, 1, NORM_MINMAX);

            //convert para float 32
            output.convertTo(output, CV_32F);
            mask.convertTo(mask, CV_32F);

            // faz split e aplicanos 3 canais.
            vector<Mat> channels;
            split(output, channels);


            channels[0] = channels[0] + channels[0].mul(mask);
            channels[1] = channels[1] + channels[1].mul(mask);
            channels[2] = channels[2] + channels[2].mul(mask);

            //mistura os canais
            merge(channels, output);

            output = output / 2;

            //limites de pixels
            min(output.cols, 255, output.rows);
            max(output.cols, 0, output.rows);

            output.convertTo(output, CV_8UC3);

            split(output, channels);

            // interpola os valores
            float redValuesOriginal[] = { 0, 42, 105, 148, 185, 255 };
            float redValues[] = { 0, 28, 100, 300, 215, 255 };
            float greenValuesOriginal[] = { 0, 40, 85, 300, 165, 212, 255 };
            float greenValues[] = { 0, 25, 75, 135, 185, 230, 255 };
            float blueValuesOriginal[] = { 0, 40, 82, 125, 300, 225, 255 };
            float blueValues[] = { 0, 38, 90, 125, 160, 210, 222 };

            Mat lookupTable(1, 256, CV_8U);
            uchar* lut = lookupTable.ptr();

            // aplica a interpolacao e cria looup no canal vermelho, verde e azul
            interpolation(lut, redValues, redValuesOriginal);
           
            // aplica o mapa para cor vermelha, verde e azul
            LUT(channels[2], lookupTable, channels[2]);

            interpolation(lut, greenValues, greenValuesOriginal);

            LUT(channels[1], lookupTable, channels[1]);

            interpolation(lut, blueValues, blueValuesOriginal);

            LUT(channels[0], lookupTable, channels[0]);

            //mistura os canais
            merge(channels, output);

            // ajustes
            cvtColor(output, output, COLOR_BGR2YCrCb);

            //converte pra float32
            output.convertTo(output, CV_32F);

            //split dos canais
            split(output, channels);

            //Scale canal Y
            channels[0] = channels[0] * 1.2;

            //limite de pixel 0 e 255
            min(channels[0].cols, 255, channels[0].rows);
            max(channels[0].cols, 0, channels[0].rows);

            //mistura os canais
            merge(channels, output);

            //converte para cv_8uc3
            output.convertTo(output, CV_8UC3);

            //converte para BGR 
            cvtColor(output, output, COLOR_YCrCb2BGR);

            imgOriginal = output;
           
        
        }

        // sun
        if (iKey == '4') {

            Mat output;
                       
            output = imgOriginal.clone();

           
            vector<Mat> channels;
            split(output, channels);

            
            float redValuesOriginal[] = { 0, 60, 110, 150, 235, 255 };
            float redValues[] = { 0, 102, 185, 220, 245, 245 };
            float greenValuesOriginal[] = { 0, 68, 105, 190, 255 };
            float greenValues[] = { 0, 68, 120, 220, 255 };
            float blueValuesOriginal[] = { 0, 88, 145, 185, 255 };
            float blueValues[] = { 0, 12, 140, 212, 255 };

           
            Mat lookupTable(1, 256, CV_8U);
            uchar* lut = lookupTable.ptr();

           
            interpolation(lut, blueValues, blueValuesOriginal);
           
            LUT(channels[0], lookupTable, channels[0]);

            
            interpolation(lut, greenValues, greenValuesOriginal);
           
            LUT(channels[1], lookupTable, channels[1]);

          
            interpolation(lut, redValues, redValuesOriginal);
           
            LUT(channels[2], lookupTable, channels[2]);

           
            merge(channels, output);

            imgOriginal = output;
        
        }

        // Fire
        if (iKey == '5') {

            Mat output;

            output = imgOriginal.clone();


            vector<Mat> channels;
            split(output, channels);

                    
            float redValuesOriginal[] = { 0, 60, 110, 150, 235, 255 };
            float redValues[] = { 0, 255, 245, 235, 225, 200 };
            
            float greenValuesOriginal[] = { 0, 68, 105, 190, 255 };
            float greenValues[] = { 0, 58, 100, 200, 230 };

            float blueValuesOriginal[] = { 0, 88, 145, 185, 255 };
            float blueValues[] = { 0, 10, 120, 200, 245 };


            Mat lookupTable(1, 256, CV_8U);
            uchar* lut = lookupTable.ptr();


            interpolation(lut, blueValues, blueValuesOriginal);

            LUT(channels[0], lookupTable, channels[0]);


            interpolation(lut, greenValues, greenValuesOriginal);

            LUT(channels[1], lookupTable, channels[1]);


            interpolation(lut, redValues, redValuesOriginal);

            LUT(channels[2], lookupTable, channels[2]);


            merge(channels, output);

            imgOriginal = output;
        
        
        }

        // Purple
        if (iKey == '6') {

            Mat output;

            output = imgOriginal.clone();


            vector<Mat> channels;
            split(output, channels);


            float redValuesOriginal[] = { 0, 60, 110, 150, 235, 255 };
            float redValues[] = { 0, 102, 185, 220, 245, 245 };

            float greenValuesOriginal[] = { 0, 68, 105, 190, 255 };
            float greenValues[] = { 0, 58, 100, 200, 230 };

            float blueValuesOriginal[] = { 0, 88, 145, 185, 255 };
            float blueValues[] = { 0, 255, 245, 235, 225, 200 };


            Mat lookupTable(1, 256, CV_8U);
            uchar* lut = lookupTable.ptr();


            interpolation(lut, blueValues, blueValuesOriginal);

            LUT(channels[0], lookupTable, channels[0]);


            interpolation(lut, greenValues, greenValuesOriginal);

            LUT(channels[1], lookupTable, channels[1]);


            interpolation(lut, redValues, redValuesOriginal);

            LUT(channels[2], lookupTable, channels[2]);


            merge(channels, output);

            imgOriginal = output;


        }

        // Blue
        if (iKey == '7') {

            Mat output;

            output = imgOriginal.clone();


            vector<Mat> channels;
            split(output, channels);


            float redValuesOriginal[] = { 0, 60, 110, 150, 235, 255 };
            float redValues[] = { 0, 58, 50, 100, 115 };

            float greenValuesOriginal[] = { 0, 68, 105, 190, 255 };
            float greenValues[] = { 0, 58, 50, 100, 115 };

            float blueValuesOriginal[] = { 0, 88, 145, 185, 255 };
            float blueValues[] = { 0, 255, 255, 255, 255, 255 };


            Mat lookupTable(1, 256, CV_8U);
            uchar* lut = lookupTable.ptr();


            interpolation(lut, blueValues, blueValuesOriginal);

            LUT(channels[0], lookupTable, channels[0]);


            interpolation(lut, greenValues, greenValuesOriginal);

            LUT(channels[1], lookupTable, channels[1]);


            interpolation(lut, redValues, redValuesOriginal);

            LUT(channels[2], lookupTable, channels[2]);


            merge(channels, output);

            imgOriginal = output;


        }

        // Green day
        if (iKey == '8') {

            Mat output;

            output = imgOriginal.clone();


            vector<Mat> channels;
            split(output, channels);


            float redValuesOriginal[] = { 0, 60, 110, 150, 235, 255 };
            float redValues[] = { 0, 58, 50, 100, 115 };

            float greenValuesOriginal[] = { 0, 68, 105, 190, 255 };
            float greenValues[] = { 0, 58, 100, 200, 230 };

            float blueValuesOriginal[] = { 0, 88, 145, 185, 255 };
            float blueValues[] = { 0, 10, 120, 200, 245 };


            Mat lookupTable(1, 256, CV_8U);
            uchar* lut = lookupTable.ptr();


            interpolation(lut, blueValues, blueValuesOriginal);

            LUT(channels[0], lookupTable, channels[0]);


            interpolation(lut, greenValues, greenValuesOriginal);

            LUT(channels[1], lookupTable, channels[1]);


            interpolation(lut, redValues, redValuesOriginal);

            LUT(channels[2], lookupTable, channels[2]);


            merge(channels, output);

            imgOriginal = output;


        }

        // Ajusta a gramatura da imagem
        if (iKey == '9') {

            Mat output;

            //define gamma
            float gamma = 1.5;

            Mat lookupTable(1, 256, CV_8U);
            uchar* lut = lookupTable.ptr();
            for (int i = 0; i < 256; i++) {
                lut[i] = int(255 * (pow((float)i / 255.0, gamma)));
            }
            cout << lookupTable << endl;
                        
            LUT(imgOriginal, lookupTable, output);

            imgOriginal = output;


        }
        
      
        
    }



    //chama a fun??o para esperar o usuario apertar uma tecla, para n?o fechar antes.
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

Fun??es:

- drawText definida para inserir um texto na imagem via teclado.

- carregaImagem e carregaImagemStickers definidas para carregar uma imagem via console.
Vari?vel path do tipo string cont?m o caminho do diret?rio de imagens,
com itera??o para listar o nome das imagens dispon?veis.

- OverlayImage definida para percorrer todos os pixel do sticker e colar na imagem.

- mouseCallback definida para mapear e inserir sticker na imagem com overlayImage.

- Interpolacao definida para alterar os canais de cores da imagem dentro dos limites.

- on_tackbar definida para aplica??o do video.

- Menu de op??es para guia de usu?rio.

*/

void drawText(Mat& image)
{
    string text;
    cout << "Digite o texto desejado: ";
    getline(cin, text);

    putText(image, text,
        Point(20, 50),
        FONT_HERSHEY_COMPLEX, 1, 
        Scalar(0, 0, 0), 
        1, LINE_AA); 

    
}

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

    

    if (evt == EVENT_LBUTTONDOWN) { // Quando o bot?o da esquerda for clicado
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
        
        cvtColor(imgOriginal, imgOriginal, COLOR_BGR2BGRA);
        overlayImage(&imgOriginal, &sticker, Point(x - sticker.cols/2, y - sticker.rows/2));
        //sticker.copyTo(imgOriginal.rowRange(x, x + sticker.cols).colRange(y, y + sticker.rows));
      
    }
    else {
        if (evt == EVENT_RBUTTONDOWN) { // Quando o bot?o da direita for clicado
        cout << "Direita clicado, posicao:(" << x << "," << y << ")" << endl;

        
        }
        else {
            if (evt == EVENT_MBUTTONDOWN) { // Quando o bot?o do meio for clicado
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

void interpolation(uchar* lut, float* curve, float* originalValue) {
    for (int i = 0; i < 256; i++) {
        int j = 0;
        float a = i;
        while (a > originalValue[j]) {
            j++;
        }
        if (a == originalValue[j]) {
            lut[i] = curve[j];
            continue;
        }
        float slope = ((float)(curve[j] - curve[j - 1])) / ((float)(originalValue[j] - originalValue[j - 1]));
        float constant = curve[j] - slope * originalValue[j];
        lut[i] = slope * a + constant;
    }
}

void on_trackbar(int, void*)
{
    printf("%d\n", g_slider);
}

/*

Fun??o de Menu apenas para indicar os comandos dispon?veis. 

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
    printf("        I - Mouse CallBack - Sticker - charuto \n");
    printf("\n");
    printf("        J - Mouse CallBack - Sticker - oculos \n");
    printf("\n");
    printf("        M - Mouse CallBack - Sticker Nariz \n");
    printf("\n");
    printf("        O - Sticker com OverlayImage e coordenadas via teclado \n");
    printf("\n");
    printf("        R - Sticker com Coordenada via input Nariz \n");
    printf("\n");
    printf("        S - Salvar imagem \n");
    printf("\n");
    printf("        V - Execucao de video e manipulacao do trackbar \n");
    printf("\n");
    printf("        X - Salvando um frame \n");
    printf("\n");
    printf("        0 - Retira o ruido da imagem \n");
    printf("\n");
    printf("        1 - Cartoon \n");
    printf("\n");
    printf("        2 - Pencil \n");
    printf("\n");
    printf("        3 - Paraiso artificial - interpolacao \n");
    printf("\n");
    printf("        4 - sun \n");
    printf("\n");
    printf("        5 - Fire \n");
    printf("\n");
    printf("        6 - Purple \n");
    printf("\n");
    printf("        7 - Blue \n");
    printf("\n");
    printf("        8 - Green day \n");
    printf("\n");
    printf("        9 - Ajusta a gramatura da imagem \n");
    printf("\n");


}

