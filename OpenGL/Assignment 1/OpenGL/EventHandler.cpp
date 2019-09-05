//#include <windows.h>
//#include <glut.h>
//
//extern double cameraHeight;
//extern double cameraAngle;
//extern int drawgrid;
//extern int drawaxes;
//extern double angle;
//
//
//void mouseListener(int button, int state, int x, int y) {	//x, y is the x-y of the screen (2D)
//	switch (button) {
//	case GLUT_LEFT_BUTTON:
//		if (state == GLUT_DOWN) {		// 2 times?? in ONE click? -- solution is checking DOWN or UP
//			drawaxes = 1 - drawaxes;
//		}
//		break;
//
//	case GLUT_RIGHT_BUTTON:
//		//........
//		break;
//
//	case GLUT_MIDDLE_BUTTON:
//		//........
//		break;
//
//	default:
//		break;
//	}
//}
//
//void specialKeyListener(int key, int x, int y) {
//	switch (key) {
//	case GLUT_KEY_DOWN:		//down arrow key
//		cameraHeight -= 3.0;
//		break;
//	case GLUT_KEY_UP:		// up arrow key
//		cameraHeight += 3.0;
//		break;
//
//	case GLUT_KEY_RIGHT:
//		cameraAngle += 0.03;
//		break;
//	case GLUT_KEY_LEFT:
//		cameraAngle -= 0.03;
//		break;
//
//	case GLUT_KEY_PAGE_UP:
//		break;
//	case GLUT_KEY_PAGE_DOWN:
//		break;
//
//	case GLUT_KEY_INSERT:
//		break;
//
//	case GLUT_KEY_HOME:
//		break;
//	case GLUT_KEY_END:
//		break;
//
//	default:
//		break;
//	}
//}
//
//void keyboardListener(unsigned char key, int x, int y) {
//	switch (key) {
//
//	case '1':
//		drawgrid = 1 - drawgrid;
//		break;
//
//	default:
//		break;
//	}
//}
