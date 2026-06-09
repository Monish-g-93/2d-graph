#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define WIDTH 80
#define HEIGHT 24
#define MAX_OBJECTS 50

typedef enum {
    OBJ_CIRCLE,
    OBJ_RECTANGLE,
    OBJ_LINE,
    OBJ_NONE
} ObjectType;

typedef struct {
    ObjectType type;
    int x1, y1;
    int x2, y2;
    int radius;
    int id;
    int active;
} GraphicObject;

char canvas[HEIGHT][WIDTH];
GraphicObject objects[MAX_OBJECTS];
int nextId = 1;

void initCanvas(void) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            canvas[y][x] = '_';
        }
    }
}

void displayCanvas(void) {
    printf("\n");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            putchar(canvas[y][x]);
        }
        putchar('\n');
    }
}

void clearCanvas(void) {
    initCanvas();
}

void drawPixel(int x, int y) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        canvas[y][x] = '*';
    }
}

void drawCircleOnCanvas(int cx, int cy, int radius) {
    if (radius < 0) return;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            double dx = x - cx;
            double dy = y - cy;
            double dist = sqrt(dx * dx + dy * dy);
            if (fabs(dist - radius) <= 0.8) {
                drawPixel(x, y);
            }
        }
    }
}

void drawRectangleOnCanvas(int x1, int y1, int x2, int y2) {
    int left = x1 < x2 ? x1 : x2;
    int right = x1 < x2 ? x2 : x1;
    int top = y1 < y2 ? y1 : y2;
    int bottom = y1 < y2 ? y2 : y1;
    for (int x = left; x <= right; x++) {
        drawPixel(x, top);
        drawPixel(x, bottom);
    }
    for (int y = top; y <= bottom; y++) {
        drawPixel(left, y);
        drawPixel(right, y);
    }
}

void drawLineOnCanvas(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;
    int x = x1;
    int y = y1;

    while (1) {
        drawPixel(x, y);
        if (x == x2 && y == y2) break;
        int e2 = err * 2;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

void renderObjects(void) {
    clearCanvas();
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (!objects[i].active) continue;
        switch (objects[i].type) {
            case OBJ_CIRCLE:
                drawCircleOnCanvas(objects[i].x1, objects[i].y1, objects[i].radius);
                break;
            case OBJ_RECTANGLE:
                drawRectangleOnCanvas(objects[i].x1, objects[i].y1, objects[i].x2, objects[i].y2);
                break;
            case OBJ_LINE:
                drawLineOnCanvas(objects[i].x1, objects[i].y1, objects[i].x2, objects[i].y2);
                break;
            default:
                break;
        }
    }
}

int findObjectIndexById(int id) {
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (objects[i].active && objects[i].id == id) {
            return i;
        }
    }
    return -1;
}

void addCircle(void) {
    int cx, cy, r;
    printf("Enter circle center x y and radius: ");
    if (scanf("%d %d %d", &cx, &cy, &r) != 3) {
        while (getchar() != '\n');
        printf("Invalid input.\n");
        return;
    }
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (!objects[i].active) {
            objects[i].active = 1;
            objects[i].type = OBJ_CIRCLE;
            objects[i].x1 = cx;
            objects[i].y1 = cy;
            objects[i].radius = r;
            objects[i].id = nextId++;
            printf("Added circle object id=%d\n", objects[i].id);
            renderObjects();
            return;
        }
    }
    printf("Object list is full. Cannot add more objects.\n");
}

void addRectangle(void) {
    int x1, y1, x2, y2;
    printf("Enter rectangle x1 y1 x2 y2: ");
    if (scanf("%d %d %d %d", &x1, &y1, &x2, &y2) != 4) {
        while (getchar() != '\n');
        printf("Invalid input.\n");
        return;
    }
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (!objects[i].active) {
            objects[i].active = 1;
            objects[i].type = OBJ_RECTANGLE;
            objects[i].x1 = x1;
            objects[i].y1 = y1;
            objects[i].x2 = x2;
            objects[i].y2 = y2;
            objects[i].id = nextId++;
            printf("Added rectangle object id=%d\n", objects[i].id);
            renderObjects();
            return;
        }
    }
    printf("Object list is full. Cannot add more objects.\n");
}

void addLine(void) {
    int x1, y1, x2, y2;
    printf("Enter line x1 y1 x2 y2: ");
    if (scanf("%d %d %d %d", &x1, &y1, &x2, &y2) != 4) {
        while (getchar() != '\n');
        printf("Invalid input.\n");
        return;
    }
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (!objects[i].active) {
            objects[i].active = 1;
            objects[i].type = OBJ_LINE;
            objects[i].x1 = x1;
            objects[i].y1 = y1;
            objects[i].x2 = x2;
            objects[i].y2 = y2;
            objects[i].id = nextId++;
            printf("Added line object id=%d\n", objects[i].id);
            renderObjects();
            return;
        }
    }
    printf("Object list is full. Cannot add more objects.\n");
}

void deleteObject(void) {
    int id;
    printf("Enter object id to delete: ");
    if (scanf("%d", &id) != 1) {
        while (getchar() != '\n');
        printf("Invalid input.\n");
        return;
    }
    int index = findObjectIndexById(id);
    if (index < 0) {
        printf("Object id %d not found.\n", id);
        return;
    }
    objects[index].active = 0;
    printf("Deleted object id=%d\n", id);
    renderObjects();
}

void modifyObject(void) {
    int id;
    printf("Enter object id to modify: ");
    if (scanf("%d", &id) != 1) {
        while (getchar() != '\n');
        printf("Invalid input.\n");
        return;
    }
    int index = findObjectIndexById(id);
    if (index < 0) {
        printf("Object id %d not found.\n", id);
        return;
    }
    GraphicObject *obj = &objects[index];
    if (obj->type == OBJ_CIRCLE) {
        int cx, cy, r;
        printf("Enter new circle center x y and radius: ");
        if (scanf("%d %d %d", &cx, &cy, &r) != 3) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            return;
        }
        obj->x1 = cx;
        obj->y1 = cy;
        obj->radius = r;
    } else if (obj->type == OBJ_RECTANGLE) {
        int x1, y1, x2, y2;
        printf("Enter new rectangle x1 y1 x2 y2: ");
        if (scanf("%d %d %d %d", &x1, &y1, &x2, &y2) != 4) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            return;
        }
        obj->x1 = x1;
        obj->y1 = y1;
        obj->x2 = x2;
        obj->y2 = y2;
    } else if (obj->type == OBJ_LINE) {
        int x1, y1, x2, y2;
        printf("Enter new line x1 y1 x2 y2: ");
        if (scanf("%d %d %d %d", &x1, &y1, &x2, &y2) != 4) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            return;
        }
        obj->x1 = x1;
        obj->y1 = y1;
        obj->x2 = x2;
        obj->y2 = y2;
    }
    printf("Modified object id=%d\n", id);
    renderObjects();
}

void listObjects(void) {
    printf("Active objects:\n");
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (!objects[i].active) continue;
        printf(" id=%d type=", objects[i].id);
        switch (objects[i].type) {
            case OBJ_CIRCLE:
                printf("CIRCLE center=(%d,%d) r=%d\n", objects[i].x1, objects[i].y1, objects[i].radius);
                break;
            case OBJ_RECTANGLE:
                printf("RECTANGLE (%d,%d)-(%d,%d)\n", objects[i].x1, objects[i].y1, objects[i].x2, objects[i].y2);
                break;
            case OBJ_LINE:
                printf("LINE (%d,%d)-(%d,%d)\n", objects[i].x1, objects[i].y1, objects[i].x2, objects[i].y2);
                break;
            default:
                printf("UNKNOWN\n");
                break;
        }
    }
}

void showMenu(void) {
    printf("\n2D ASCII Graphics Editor\n");
    printf("1. Add circle\n");
    printf("2. Add rectangle\n");
    printf("3. Add line\n");
    printf("4. Delete object\n");
    printf("5. Modify object\n");
    printf("6. List objects\n");
    printf("7. Display picture\n");
    printf("8. Clear picture\n");
    printf("9. Exit\n");
    printf("Choose option: ");
}

int main(void) {
    initCanvas();
    memset(objects, 0, sizeof(objects));
    int choice;

    while (1) {
        showMenu();
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid entry. Please type a number.\n");
            continue;
        }
        switch (choice) {
            case 1:
                addCircle();
                break;
            case 2:
                addRectangle();
                break;
            case 3:
                addLine();
                break;
            case 4:
                deleteObject();
                break;
            case 5:
                modifyObject();
                break;
            case 6:
                listObjects();
                break;
            case 7:
                displayCanvas();
                break;
            case 8:
                for (int i = 0; i < MAX_OBJECTS; i++) objects[i].active = 0;
                renderObjects();
                printf("Canvas cleared.\n");
                break;
            case 9:
                printf("Exiting editor.\n");
                return 0;
            default:
                printf("Invalid option.\n");
        }
    }
}
