#include <iostream>
#include <sstream>
#include <string>
#include "MVCameraInput.h"
#include "VideoInput.h"
#include "WindmillDetect.h"
#include "AngleSolve.h"

#ifdef WINDOWS
#include "MessageSend.h"
#endif

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

int main(int argc, char* argv[]) {

#ifdef DEBUG
    cv::namedWindow("show", cv::WINDOW_NORMAL);
    cv::namedWindow("gray", cv::WINDOW_NORMAL);
    cv::namedWindow("binary", cv::WINDOW_NORMAL);
    // cv::namedWindow("fan", cv::WINDOW_NORMAL);
#endif

    WindmillDetect inst;

    AngleSolve solve;
    
    #ifdef WINDOWS
        MessageSend send;
    #endif

    int structElementSize = 1;
    inst.setElement(getStructuringElement(
            cv::MORPH_RECT,
            cv::Size(2 * structElementSize + 1, 2 * structElementSize + 1),
            cv::Point(structElementSize, structElementSize)));

    //MVCameraInput video;
    VideoInput video;

    if (!(video.isOpened())) return 0;

    cv::Mat frame;
    video >> frame;

    inst.process(frame);

    const int process_width = 640;
    video.setResolution(cv::Size(process_width, static_cast<double>(process_width) / frame.cols * frame.rows));

    while (!frame.empty()) {
#ifdef DEBUG
        cv::imshow("gray", inst.gray);
        cv::imshow("binary", inst.binary);
        cv::imshow("show", inst.show);
        if (cv::waitKey() == 27) break;
#endif
        double t = cv::getTickCount();
        auto aimArea = inst.process(frame);
        std::cout << "Process taken " << 1000 * (cv::getTickCount() - t) / cv::getTickFrequency() << " ms \n";
        //cv::waitKey(10);

        #ifdef WINDOWS
            if (aimArea.empty()) {
                std::cout << "\033[31mNo target found!\033[0m\n";
            }

            else { 
                std::cout << "aimArea: " << arrayToString(aimArea) << "\n";
                auto [yaw, pitch] = solve.getAngle(aimArea);
                std::cout << yaw << " " << pitch << "\n";
                send.sendMessage(false, yaw, pitch);
                std::cout << send.getData() << "\n";
            }
        #endif
        video >> frame;
    }
}
