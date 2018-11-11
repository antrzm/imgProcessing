#ifndef IMAGE_EDITOR_H
#define IMAGE_EDITOR_H

class ImageEditor {
public:
    virtual void gauss(int window, float sigma) = 0;
    virtual void sobel()  = 0;
    virtual void gabor(int window, float tetta) = 0;
    virtual void canny()  = 0;
    virtual void otsu()  = 0;
    virtual void hsv_transform(float h_shift, float s_mul, float v_mul) = 0;
    virtual void try_hsv_transform(float h_shift, float s_mul, float v_mul) = 0;
    virtual void update_image() = 0;
    virtual void reset_image() = 0;
    virtual void finished() = 0;
};

#endif // IMAGE_EDITOR_H
