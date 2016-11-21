import sys
import cv2
import lime
import numpy as np
import matplotlib.pyplot as plt

def main():
    args = sys.argv
    if len(args) <= 1:
        print('usage: demo_color_constancy [ input image ]')
        sys.exit(1)

    img = cv2.imread(args[1])
    if img is None:
        print('Failed to load image: {}'.format(args[1]))
        sys.exit(1)

    img = (img / 255.0).astype(np.float32)

    out_horn = lime.colorConstancy(img, lime.CONSTANCY_HORN)
    out_rahman = lime.colorConstancy(img, lime.CONSTANCY_RAHMAN)
    out_faugeras = lime.colorConstancy(img, lime.CONSTANCY_FAUGERAS)

    fig, axarr = plt.subplots(1, 4, figsize=(12, 3))

    axarr[0].imshow(cv2.cvtColor(img, cv2.COLOR_BGR2RGB))
    axarr[0].axis('off')
    axarr[0].set_title('Input')

    axarr[1].imshow(cv2.cvtColor(out_horn, cv2.COLOR_BGR2RGB))
    axarr[1].axis('off')
    axarr[1].set_title('Horn')

    axarr[2].imshow(cv2.cvtColor(out_rahman, cv2.COLOR_BGR2RGB))
    axarr[2].axis('off')
    axarr[2].set_title('Rahman')

    axarr[3].imshow(cv2.cvtColor(out_faugeras, cv2.COLOR_BGR2RGB))
    axarr[3].axis('off')
    axarr[3].set_title('Faugeras')

    plt.show()

if __name__ == '__main__':
    main()
