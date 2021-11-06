#include "MVCameraInput.h"
#include "ArmorDetect.h"
#include "AngleSolve.h"
#ifdef WINDOWS
#include "MessageSend.h"
#endif

template<class Iterable>
static std::string arrayToString(Iterable t) {
    std::string str = "[";
    for (const auto& val : t) { str += to_string(val) + ", "; }
    if (str.length() > 2) {
        str.pop_back();
        str.pop_back();
    }
    return str + "]";
}


int main(int argc, char *argv[])
{
    cv::namedWindow("frame");
    cv::namedWindow("show");

    #ifdef WINDOWS
    cv::VideoCapture cap(1);
    if(!cap.isOpened())
    {
        std::cout << " can't open camera "<< std::endl;
    }
    #else
        MVCameraInput cap;
    #endif
    
    AngleSolve solve;
    
    #ifdef WINDOWS
        MessageSend send;
    #endif
    cv::Mat frame;
    cap >> frame;
    std::vector<cv::Point2f> final;
    int yaw=0;
    int pitch=0;
    std::cout << frame.size();
    double fps;
    double t = 0;
    char string[10];            // ���֡�ʵ��ַ���
    
    //cv::waitKey(0);

    ArmorDetect inst(ArmorDetect::Mode::FIND_BLUE);

    while (true)
    {
        t = (double)cv::getTickCount();
        cv::waitKey(10);
        cap >> frame;
        
        cv::imshow("frame", frame);

        cv::Mat show;
        frame.copyTo(show);
        
        // for (const auto &bar : inst.getLightBars())
        // {
        //     cv::Point2f pts[4];
        //     bar.box.points(pts);
        //     drawTetragon(show, pts, cv::Scalar(0, 255, 0));
        // }

        cv::Point2f aimArea[4];
        inst.process(frame,aimArea);
        if (!aimArea)
            std::cout << "No target found.\n";
        else{
            drawTetragon(show, aimArea, cv::Scalar(0, 255, 255));
            
        }

        for (const auto &armor : inst.getResults())
        {
            cv::Point2f pts[4];
            armor.points(pts);
            drawTetragon(show, pts, cv::Scalar(0, 255, 0));
        }
        
        t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
        fps = 1.0 / t;
        sprintf(string, "%.2f", fps);      
        std::string fpsString("FPS:");
        fpsString += string;      
        putText(show,                   // ͼ�����
            fpsString,                  // string����������
            cv::Point(5, 20),           // �������꣬�����½�Ϊԭ��
            cv::FONT_HERSHEY_SIMPLEX,   // ��������
            0.5, // �����С
            cv::Scalar(255, 255, 255));       // ������ɫ
            cv::imshow("show", show);
        if (cv::waitKey(30) == 27) break;
        //#ifdef WINDOWS
            if (!aimArea) {
                std::cout << "\033[31mNo target found!\033[0m\n";
            }
            else { 
                for(int i=0;i<4;i++){
                    //for(int j=0;j<2;j++){
                        final[i] = aimArea[i];
                    //}
                }
                std::cout << "aimArea: " << arrayToString(aimArea) << "\n";
                auto [yaw, pitch] = solve.getAngle(final);
                std::cout << yaw << " " << pitch << "\n";
                send.sendMessage(false, yaw, pitch);
                std::cout << send.getData() << "\n";
            }
        //#endif
    }
}
