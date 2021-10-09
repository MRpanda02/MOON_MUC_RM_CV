//
// Created by Henry on 2021/5/23.
//

#ifndef MUC_CLEMENTINE_ARMORDETECT_H
#define MUC_CLEMENTINE_ARMORDETECT_H

#include <iostream>
#include <vector>
#include "Armor.h"

using ContoursVector = std::vector< std::vector< cv::Point > >;

struct ArmorDetectParams {
    int highlightThresholdForRed    = 200;
    int highlightThresholdForBlue   = 200;
    int colorThresholdForRed        = 30;
    int colorThresholdForBlue       = 30;
    float lightBarsCenterMaxDiffY   = 20; // 0.05 * frame height
    float minLightBarArea           = 15;
};

class ArmorDetect {
public:
    enum class Mode { FIND_RED, FIND_BLUE };

    explicit ArmorDetect(Mode m) : mode(m) {}

    void process(const cv::Mat& frame,cv::Point2f aimArea[]) {
        if (this->mode == Mode::FIND_RED) {
            // 修改
            cv::Mat color = findRedPreprocess(frame);
            // 修改
            lightBars = findLightBars(color);
            // 修改
            results = lightBarsPairing(lightBars);
        } else {
            // 修改
            cv::Mat color = findBluePreprocess(frame);
            lightBars = findLightBars(color);
            results = lightBarsPairing(lightBars);
        }

        if (results.empty()) {
            lastTarget = Armor();
        } else {
            cv::Point2f pts[4];
            lastTarget = choice(results);
            lastTarget.points(pts);
            for(int i=0;i<4;i++)
            {
                aimArea[i] = pts[i];
            }
            std::cout << "( " ;
            for(int i=0;i<4;i++){
                std::cout << aimArea[i] << " , ";
            }
            std::cout << " ) ";
        }
    }

    void setParams(ArmorDetectParams p) { params = p; }

    [[nodiscard]] const std::vector<Armor> & getResults() const { return results; }
    [[nodiscard]] const std::vector<LightBar> & getLightBars() const { return lightBars; }

private:
    std::vector<LightBar> lightBars;
    std::vector<Armor> results;

    Armor choice(const std::vector<Armor>& armors);

    Armor lastTarget;

    [[nodiscard]] cv::Mat findRedPreprocess(const cv::Mat& frame) const;
    [[nodiscard]] cv::Mat findBluePreprocess(const cv::Mat& frame) const;
    [[nodiscard]] std::vector<LightBar> findLightBars(const cv::Mat& color) const;
    [[nodiscard]] std::vector<Armor> lightBarsPairing(const std::vector<LightBar>& lightBars) const;

    Mode mode;

    ArmorDetectParams params;
};

void drawTetragon(cv::Mat &image, cv::Point2f *vertices, const cv::Scalar &color);

#endif //MUC_CLEMENTINE_ARMORDETECT_H
