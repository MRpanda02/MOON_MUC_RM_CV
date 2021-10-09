//
// Created by Henry on 2021/5/23.
//

#include "ArmorDetect.h"
#include "Energy.h"
#include "svm.h"

void drawTetragon(cv::Mat &image, cv::Point2f *vertices, const cv::Scalar &color) {
    using cv::Scalar;
    int thickness = ceil(2e-3 * image.cols);
    for (int j = 0; j < 4; j++) {
        cv::line(image, vertices[j], vertices[(j + 1) % 4], color, thickness);
    }
    int radius = ceil(1e-2 * image.cols);
    // cv::circle(image, vertices[0], radius, Scalar(  0,   0, 255), -1); // red
    // cv::circle(image, vertices[1], radius, Scalar(  0, 255, 255), -1); // yellow
    // cv::circle(image, vertices[2], radius, Scalar(255,   0, 255), -1); // purple / violet
    // cv::circle(image, vertices[3], radius, Scalar(255,   0,   0), -1); // blue
}

cv::Mat ArmorDetect::findRedPreprocess(const cv::Mat& frame) const {
    Mat srcImg_binary = Mat::zeros(frame.size(), CV_8UC1); //color feature image
    namedWindow("srcImg_binary",WINDOW_FREERATIO);
	//pointer visits all the data of srcImg, the same to bgr channel split 通道相减法的自定义形式，利用指针访问，免去了split、substract和thresh操作，加速了1.7倍
	//data of Mat  bgr bgr bgr bgr
	uchar *pdata = (uchar*)frame.data;
	uchar *qdata = (uchar*)srcImg_binary.data;
	int srcData = frame.rows * frame.cols;
	for (int i = 0; i < srcData; i++)
	{
		if (*(pdata + 2) - *pdata > 80)
			*qdata = 255;
		pdata += 3;
		qdata++;
	}
    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3)); //kernel for dilate;  shape:ellipse size:Size(3,3) 膨胀操作使用的掩膜
	dilate(srcImg_binary, srcImg_binary, kernel); //dilate the roiImg_binary which can make the lightBar area more smooth 对roiIng_binary进行膨胀操作，试得灯条区域更加平滑有衔接
    imshow("srcImg_binary",srcImg_binary);
    return srcImg_binary;
}

cv::Mat ArmorDetect::findBluePreprocess(const cv::Mat& frame) const {
    Mat srcImg_binary = Mat::zeros(frame.size(), CV_8UC1); //color feature image
    namedWindow("srcImg_binary",WINDOW_FREERATIO);
	//pointer visits all the data of srcImg, the same to bgr channel split 通道相减法的自定义形式，利用指针访问，免去了split、substract和thresh操作，加速了1.7倍
	//data of Mat  bgr bgr bgr bgr
	uchar *pdata = (uchar*)frame.data;
	uchar *qdata = (uchar*)srcImg_binary.data;
	int srcData = frame.rows * frame.cols;
	for (int i = 0; i < srcData; i++)
	{
		if (*(pdata) - *(pdata + 2) > 80)
			*qdata = 255;
		pdata += 3;
		qdata++;
	}
    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3)); //kernel for dilate;  shape:ellipse size:Size(3,3) 膨胀操作使用的掩膜
	dilate(srcImg_binary, srcImg_binary, kernel); //dilate the roiImg_binary which can make the lightBar area more smooth 对roiIng_binary进行膨胀操作，试得灯条区域更加平滑有衔接
    imshow("srcImg_binary",srcImg_binary);
    return srcImg_binary;
}

std::vector<LightBar> ArmorDetect::findLightBars(const cv::Mat& color) const 
{
    ContoursVector colorContours;

    cv::findContours(color, colorContours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::RotatedRect> colorBoundingRRects;

    for (const auto& c : colorContours) {
        colorBoundingRRects.push_back(cv::minAreaRect(c));
    }

    std::vector<LightBar> lightBars;

    auto isInside = [](const cv::Rect& a, const cv::Rect& b) { return (a == (a & b)); };

    // ---
    //  cv::Mat show(cv::Size(848, 586), CV_8UC3);
    // ---


    for (auto&& h : colorContours) {

        auto box = cv::minAreaRect(h);
        auto boxArea = box.size.width * box.size.height;

        if (boxArea < this->params.minLightBarArea) continue;

        auto width = box.size.width;
        auto height = box.size.height;
        auto angle = box.angle;


        if (height > width)  { // light bar like: '//'
            angle = 90 - angle;
        } else { // light bar like: '\\'
            std::swap(height, width);
            angle = 180 - angle;
        }

        auto ratio = height / width;

        if (ratio > Armor::maxLightBarLengthWidthRatio) continue;
        if (angle < Armor::lightBarMinAngle || angle > Armor::lightBarMaxAngle) continue;

        bool inside = false;

        for (const auto& r : colorBoundingRRects) {
            std::vector<cv::Point2f> intersectingRegion;
            if (cv::rotatedRectangleIntersection(r, box, intersectingRegion) != cv::INTERSECT_NONE) {
                inside = true;
                break;
            }
        }
        if (inside) {
            lightBars.emplace_back(box, angle, height, width);
        }
    }

    return lightBars;
}

std::vector<Armor> ArmorDetect::lightBarsPairing(const std::vector<LightBar> &lightBars) const {
    if (lightBars.size() < 2) return std::vector<Armor>();

    std::vector<Armor> armors;

    for (auto i = 0; i < lightBars.size() - 1; i++) {
        for (auto j = i + 1; j < lightBars.size(); j++) {
            auto maxBarLength = std::max(lightBars[i].length, lightBars[j].length);
            //两灯条夹角的差
            if (abs(lightBars[i].angle - lightBars[j].angle) > Armor::maxAngleBetweenLightBars) continue; 

            float delta_x = lightBars[j].center.x - lightBars[i].center.x; //Δx
	        float delta_y = lightBars[j].center.y - lightBars[i].center.y; //Δy
	        float deviationAngle = abs(atan(delta_y / delta_x)) * 180 / CV_PI; //tanθ=Δy/Δx
            if(deviationAngle > 50) continue;
            // 两灯条中心x方向差距比值
            float meanLen = (lightBars[i].length + lightBars[j].length) / 2;
	        float xDiff = abs(lightBars[i].center.x - lightBars[j].center.x); //x distance ration y轴方向上的距离比值（y轴距离与灯条平均值的比）
	        float xDiff_ratio = xDiff / meanLen;
            if(xDiff_ratio > 6 ) continue;
            // if (abs(lightBars[i].length - lightBars[j].length) / maxBarLength > 0.5) continue; // 两灯条长度的差
	        float yDiff = abs(lightBars[i].center.y - lightBars[j].center.y); //x distance ration y轴方向上的距离比值（y轴距离与灯条平均值的比）
	        float yDiff_ratio = yDiff / meanLen;
            if(yDiff_ratio > 0.5 ) continue;
            // if (abs(lightBars[i].center.x - lightBars[j].center.x) / maxBarLength > Armor::maxArmorWidthToLightBarLength) continue;
            // if (abs(lightBars[i].center.y - lightBars[j].center.y) > this->params.lightBarsCenterMaxDiffY) continue;
            if (abs(lightBars[i].length - lightBars[j].length) / maxBarLength > 0.5) continue; // 两灯条长度的差
            armors.emplace_back(lightBars[i], lightBars[j]);

            // SVM操作
            
            
            // cv::Point2f pts[4];
            // lightBars[i].box.points(pts);
            // drawTetragon(frame, pts, cv::Scalar(0, 255, 0));
            // lightBars[j].box.points(pts);
            // drawTetragon(frame, pts, cv::Scalar(0, 255, 0));
        }
    }
    return armors;
}

Armor ArmorDetect::choice(const std::vector<Armor>& armors) {
    return armors[0];
}

// void Energy::run(cv::Mat& src) {
// 	clearAll();
// 	initImage(src);
// 	if (show_process)imshow("bin", src);
// 	if (findArmors(src) < 1)return;//寻找所有大风车装甲板
// 	if (show_energy) showArmors("armor", src);
// 	if (!findFlowStripFan(src)) {//寻找图中流动条所在的扇叶
// 		if (!findFlowStripWeak(src)) return;
// 	}
// 	else {
// 		if (show_energy)showFlowStripFan("strip fan", src);
// 		if (!findTargetInFlowStripFan()) return;//在流动条区域内寻找装甲板
// 		if (!findFlowStrip(src)) return;//寻找流动条
// 	}
// 	if (show_energy) showFlowStrip("strip", src);
// 	findCenterROI(src);//寻找R标范围(缩小R标检测范围，可提高检测速度，降低误识别率）
// 	if (!findCenterR(src)) return;//寻找中心R
// 	if (show_energy) showCenterR("R", src);
// 	fans_cnt = findFans(src);//寻找图像中所有扇叶
// 	if (show_energy) showFans("fans", src);

// 	getTargetPolarAngle();//将待打击装甲板坐标转化为极坐标
// 	if (energy_rotation_init) {
// 		initRotation();
// 		return;
// 	}
// 	getPredictPoint(target_point);
// 	if (show_energy) showPredictPoint("Predict", src);
// 	if (show_data) showData(target_point,predict_point,circle_center_point);
// }
