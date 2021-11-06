#include "MVCameraInput.h"
#include "ArmorDetect.h"
#include "AngleSolve.h"
#include "MessageSend.h"

std::string to_string(cv::Point2f p) {
    std::ostringstream oss;
    oss << "(" << std::setprecision(5) << p.x << "," << p.y << ")";
    return oss.str();
}

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
    
    MessageSend send;

    cv::Mat frame;
    cap >> frame;
    int yaw=0;
    int pitch=0;
    std::cout << frame.size();
    double fps;
    double t = 0;
    char string[10];            // 存放帧率的字符串
    
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
        auto final = inst.process(frame,aimArea);
        if (final.empty())
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
        putText(show,                   // 图像矩阵
            fpsString,                  // string型文字内容
            cv::Point(5, 20),           // 文字坐标，以左下角为原点
            cv::FONT_HERSHEY_SIMPLEX,   // 字体类型
            0.5, // 字体大小
            cv::Scalar(255, 255, 255));       // 字体颜色
            cv::imshow("show", show);
        //#ifdef WINDOWS
            if (final.empty()) {
                std::cout << "\033[31mNo target found!\033[0m\n";
            }
            else {
                std::cout << "aimArea: " << arrayToString(final) << "\n";
                auto [yaw, pitch] = solve.getAngle(final);
                std::cout << yaw << " " << pitch << "\n";
                send.sendMessage(false, yaw, pitch);
                std::cout << send.getData() << "\n";
            }
            if (cv::waitKey(30) == 27) break;
        //#endif
    }
}
