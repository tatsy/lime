import sys
from itertools import product
import cv2
import pylime as lime
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm

def description():
    print("*** Menu list ***")
    print("[1] Cartoon pipeline")
    print("[2] Kuwahara filter")
    print("[3] Morphology filter")
    print("[4] PDE")
    print("[5] DoG")
    print("[6] CEF")
    print("[0] Exit")

def demoCartoon(img):
    maxiter = int(input('How many bilateral filtering?: '))

    tmp = img.copy()
    for i in range(maxiter):
        bf = cv2.bilateralFilter(tmp, 9, 0.1, 5.0)
        tmp = bf.copy()
    print('  [Cartoon] bilateral filter -> OK')

    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    print(gray.shape)
    edge = lime.edgeDoG(gray, {})

    width = img.shape[1]
    height = img.shape[0]
    dim = img.shape[2]

    out = bf * np.tile(edge.reshape((height, width, 1)), (1, 1, 3))

    fig = plt.figure()
    
    ax = fig.add_subplot(2, 2, 1)
    ax.axis('off')
    ax.imshow(cv2.cvtColor(img, cv2.COLOR_BGR2RGB))
    ax.set_title('Input')

    ax = fig.add_subplot(2, 2, 2)
    ax.axis('off')
    ax.imshow(cv2.cvtColor(bf, cv2.COLOR_BGR2RGB))
    ax.set_title('Bilateral')

    ax = fig.add_subplot(2, 2, 3)
    ax.axis('off')
    ax.imshow(edge, cmap=cm.gray)
    ax.set_title('Edge')

    ax = fig.add_subplot(2, 2, 4)
    ax.axis('off')
    ax.imshow(cv2.cvtColor(out, cv2.COLOR_BGR2RGB))
    ax.set_title('Cartoon')

    plt.tight_layout()
    plt.show()

def demoKuwahara(img):
    kf = lime.kuwaharaFilter(img, lime.KUWAHARA_CLASSICAL, 7)
    print('[Kuwahara] standard kuwahara    -> OK')
    gkf = lime.kuwaharaFilter(img, lime.KUWAHARA_GENERALIZED, 7, 8)
    print('[Kuwahara] generalized kuwahara -> OK')
    akf = lime.kuwaharaFilter(img, lime.KUWAHARA_ANISOTROPIC, 7, 8)
    print('[Kuwahara] anisotropic kuwahara -> OK')

    fig = plt.figure()
    
    ax = fig.add_subplot(2, 2, 1)
    ax.axis('off')
    ax.imshow(cv2.cvtColor(img, cv2.COLOR_BGR2RGB))
    ax.set_title('Input')

    ax = fig.add_subplot(2, 2, 2)
    ax.axis('off')
    ax.imshow(cv2.cvtColor(kf, cv2.COLOR_BGR2RGB))
    ax.set_title('Classical')

    ax = fig.add_subplot(2, 2, 3)
    ax.axis('off')
    ax.imshow(cv2.cvtColor(gkf, cv2.COLOR_BGR2RGB))
    ax.set_title('Generalized')

    ax = fig.add_subplot(2, 2, 4)
    ax.axis('off')
    ax.imshow(cv2.cvtColor(akf, cv2.COLOR_BGR2RGB))
    ax.set_title('Anisotropic')

    plt.tight_layout()
    plt.show()

def demoMorphology(img):
    ksize = 5
    dilate = lime.morphFilter(img, lime.MORPH_DILATE, ksize)
    erode = lime.morphFilter(img, lime.MORPH_ERODE, ksize)
    opening = lime.morphFilter(img, lime.MORPH_OPEN, ksize)
    closing = lime.morphFilter(img, lime.MORPH_CLOSE, ksize)
    grad = lime.morphFilter(img, lime.MORPH_GRADIENT, ksize)
    tophat = lime.morphFilter(img, lime.MORPH_TOPHAT, ksize)
    blackhat = lime.morphFilter(img, lime.MORPH_BLACKHAT, ksize)

    images = [ [ img, dilate, erode, opening ],
               [ closing, grad, tophat, blackhat ] ]
    titles = [ [ 'Input', 'Dilation', 'Erosion', 'Opening' ],
               [ 'Closing', 'Gradient', 'Tophat', 'Blackhat' ] ]

    fig, axarr = plt.subplots(2, 4)
    for i, j in product(range(2), range(4)):
        axarr[i, j].axis('off')
        axarr[i, j].imshow(cv2.cvtColor(images[i][j], cv2.COLOR_BGR2RGB))
        axarr[i, j].set_title(titles[i][j])

    plt.tight_layout()
    plt.show()

def demoPDE(img):
    ad = lime.pdeFilter(img, lime.PDE_ANISO_DIFFUSION, 0.05, 10)
    print('[PDE] anisotropic diffusion -> OK')
    sf = lime.pdeFilter(img, lime.PDE_SHOCK_FILTER, 3.0, 10)
    print('[PDE] shock filter -> OK')
    mcf = lime.pdeFilter(img, lime.PDE_MEAN_CURVATURE, 3.0, 10)
    print('[PDE] mean curvature flow -> OK')

    out = img.copy()
    for i in range(5):
        out = lime.pdeFilter(out, lime.PDE_MEAN_CURVATURE, 3.0, 5)
        out = lime.pdeFilter(out, lime.PDE_SHOCK_FILTER, 3.0, 1)
    print('[PDE] SF + MCF -> OK')

    images = [ [ img, ad, None ], [ sf, mcf, out ] ]
    titles = [ [ 'Input', 'AD', None ],
               [ 'Shock', 'Mean curvature', 'SF + MCF'] ]

    fig, axarr = plt.subplots(2, 3)
    for i, j in product(range(2), range(3)):
        axarr[i, j].axis('off')
        if images[i][j] is not None and titles[i][j] is not None:
            axarr[i, j].imshow(cv2.cvtColor(images[i][j], cv2.COLOR_BGR2RGB))
            axarr[i, j].set_title(titles[i][j])

    plt.tight_layout()
    plt.show()

def demoDoG(img):
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    xdog_params = {
        'kappa': 4.5, 'sigma': 0.5, 'tau': 0.95, 'phi': 10.0,
        'edgeType': lime.NPR_EDGE_XDOG
    }
    xdog = lime.edgeDoG(gray, xdog_params)
    print('[DoG] XDoG -> OK')

    fdog_params = {
        'kappa': 2.5, 'sigma': 0.5, 'tau': 0.95, 'phi': 10.0,
        'edgeType': lime.NPR_EDGE_FDOG
    }
    fdog = lime.edgeDoG(gray, fdog_params)
    print('[DoG] FDoG -> OK')

    fig, (ax1, ax2, ax3) = plt.subplots(1, 3)

    ax1.axis('off')
    ax1.imshow(cv2.cvtColor(img, cv2.COLOR_BGR2RGB))
    ax1.set_title('Input')

    ax2.axis('off')
    ax2.imshow(xdog, cmap=cm.gray)
    ax2.set_title('XDoG')

    ax3.axis('off')
    ax3.imshow(fdog, cmap=cm.gray)
    ax3.set_title('FDoG')

    plt.tight_layout()
    plt.show()

def demoCEF(img):
    angle = lime.calcVectorField(img, 5)
    tangent = np.ndarray((angle.shape[0], angle.shape[1], 2), dtype=np.float32)
    tangent[:,:,0] = np.cos(angle) * 2.0
    tangent[:,:,1] = np.sin(angle) * 2.0

    tmp = img.copy()
    out = lime.LIC(tmp, tangent, 20, lime.LIC_RUNGE_KUTTA)
    print('[CEF] LIC Runge-Kutta -> OK')
    out = lime.pdeFilter(out, lime.PDE_SHOCK_FILTER, 3.0, 10)
    print('[CEF] Shock filter -> OK')
    out = lime.pdeFilter(out, lime.PDE_ANISO_DIFFUSION, 0.1, 5)
    print('[CEF] Edge smoothing -> OK')

    fig, (ax1, ax2) = plt.subplots(1, 2)

    ax1.axis('off')
    ax1.imshow(cv2.cvtColor(img, cv2.COLOR_BGR2RGB))
    ax1.set_title('Input')

    ax2.axis('off')
    ax2.imshow(cv2.cvtColor(out, cv2.COLOR_BGR2RGB))
    ax2.set_title('Output')

    plt.tight_layout()
    plt.show()

def main():
    args = sys.argv
    if len(args) <= 1:
        print('usage: python demo_npr_filters [ input image ]')
        sys.exit(1)

    img = cv2.imread(args[1], cv2.IMREAD_COLOR)
    if img is None:
        print('Failed to load image: {}'.format(args[1]))
        sys.exit(1)

    img = (img / 255.0).astype(np.float32)

    while True:
        description()
        demo = int(input('Please choose demo number: '))

        if demo == 0:
            print('Bye!!')
            break

        elif demo == 1:
            demoCartoon(img)

        elif demo == 2:
            demoKuwahara(img)

        elif demo == 3:
            demoMorphology(img)

        elif demo == 4:
            demoPDE(img)

        elif demo == 5:
            demoDoG(img)

        elif demo == 6:
            demoCEF(img)

        else:
            print('Unknown demo number. Try again, please.')


if __name__ == '__main__':
    main()
