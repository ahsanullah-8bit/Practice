#include <iostream>
#include <opencv2/opencv.hpp>

#include "dbpostprocess.hpp"

void prepare_prob_map(const cv::Mat& src, cv::Mat& dst) {
    // Resize the image, for more accurate testing.
    cv::resize(src, dst, cv::Size(300, 300));

    if (dst.channels() == 3) {
        cv::cvtColor(dst, dst, cv::COLOR_BGR2GRAY);
    } else {
        dst = dst;
    }

    dst.convertTo(dst, CV_32FC1, 1.0 / 255.0);
}

void draw_detection_results(cv::Mat &image, const std::vector<box_t> &boxes) {
    for (const auto &box : boxes) {
        std::vector<cv::Point> int_points;
        for (const auto &pt : box.points) {
            int_points.push_back(cv::Point(static_cast<int>(pt.x), static_cast<int>(pt.y)));
        }

        std::vector<std::vector<cv::Point>> contours = { int_points };
        cv::polylines(image, contours, true, cv::Scalar(0, 255, 0), 2, cv::LINE_AA);

        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << box.score;
        std::string label = ss.str();

        int font_face = cv::FONT_HERSHEY_SIMPLEX;
        double font_scale = 0.5;
        int thickness = 1;
        
        int baseline = 0;
        cv::Size text_size = cv::getTextSize(label, font_face, font_scale, thickness, &baseline);
        cv::Point text_org = int_points[0]; // Top-left corner
        
        cv::rectangle(image, 
                      text_org + cv::Point(0, baseline), 
                      text_org + cv::Point(text_size.width, -text_size.height), 
                      cv::Scalar(0, 0, 0), 
                      cv::FILLED);

        cv::putText(image, label, text_org, font_face, font_scale, cv::Scalar(255, 255, 255), thickness, cv::LINE_AA);
    }
}

int main(int argc, char* argv[]) {
    cv::Mat image;
    
    if (argc > 1)
        image = cv::imread(argv[1]);
    else
        image = cv::imread("assets/images/prob_map2.png");

    cv::Mat prob_map;
    prepare_prob_map(image, prob_map);
    
    const auto boxes = extract_text_boxes(prob_map, image.size());
    draw_detection_results(image, boxes);

    cv::imshow("DB", image);
    cv::waitKey();

    return 0;
}