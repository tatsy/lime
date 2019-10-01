import sys
import cv2
import pylime as lime
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

    fig = plt.figure()

    ax = fig.add_subplot(2, 2, 1)
    ax.imshow(cv2.cvtColor(img, cv2.COLOR_BGR2RGB))
    ax.axis('off')
    ax.set_title('Input')

    ax = fig.add_subplot(2, 2, 2)
    ax.imshow(cv2.cvtColor(out_horn, cv2.COLOR_BGR2RGB))
    ax.axis('off')
    ax.set_title('Horn')

    ax = fig.add_subplot(2, 2, 3)
    ax.imshow(cv2.cvtColor(out_rahman, cv2.COLOR_BGR2RGB))
    ax.axis('off')
    ax.set_title('Rahman')

    ax = fig.add_subplot(2, 2, 4)
    ax.imshow(cv2.cvtColor(out_faugeras, cv2.COLOR_BGR2RGB))
    ax.axis('off')
    ax.set_title('Faugeras')

    plt.tight_layout()
    plt.show()

if __name__ == '__main__':
    main()
