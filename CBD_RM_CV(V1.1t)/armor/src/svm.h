#include "ArmorDetect.h"

void loadSvmModel(const char * mode_path,Size armorImgSIze);

class ArmorNumClassifier
{
public:
	ArmorNumClassifier();
	~ArmorNumClassifier();

	/**
	 * @brief: load the SVM model used to recognize armorNum ����SVMģ�ͣ�����ʶ��װ�װ����֣�
	 * @param: the path of xml_file, the size of the training dataset ImgSize  ������SVMģ�͵�·�� ģ�͵�ͼƬ�ߴ�
	 */
	void loadSvmModel(const char *model_path, Size armorImgSize = Size(40, 40));

	/**
	 * @brief: load the current roiImage from ArmorDetector ����roiImage�����г�װ�װ壩
	 * @param: the path of xml_file  ������SVMģ�͵�·��
	 */
    void loadImg(Mat & srcImg);

	// /**
	//  * @brief: use warpPerspective to get armorImg  ����͸�ӱ任���װ�װ�ͼƬ
	//  * @param: the path of xml_file  ������SVMģ�͵�·��
	//  */
	// void getArmorImg(ArmorBox& armor);

	// /**
	//  * @brief: use SVM to recognize the number of each Armor ����SVMʵ��װ�װ�����ʶ��
	//  */
	// void setArmorNum(ArmorBox& armor);

private:
	Ptr<SVM>svm;  //svm model svmģ��
	Mat p;		//preRecoginze matrix for svm ���뵽SVM��ʶ��ľ���
	Size armorImgSize; //svm model training dataset size SVMģ�͵�ʶ��ͼƬ��С��ѵ������ͼƬ��С��

	Mat warpPerspective_src; //warpPerspective srcImage  ͸��任��ԭͼ
	Mat warpPerspective_dst; //warpPerspective dstImage   ͸��任���ɵ�Ŀ��ͼ
	Mat warpPerspective_mat; //warpPerspective transform matrix ͸��任�ı任����
	Point2f srcPoints[4];   //warpPerspective srcPoints		͸��任��ԭͼ�ϵ�Ŀ��� tl->tr->br->bl  ���� ���� ���� ����
	Point2f dstPoints[4];	//warpPerspective dstPoints     ͸��任��Ŀ��ͼ�еĵ�   tl->tr->br->bl  ���� ���� ���� ����
};
