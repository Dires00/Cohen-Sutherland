unsigned char code(double x, double y, double xmin, double xmax, double ymin, double ymax){
    unsigned char code = 0;

    if(y > ymax){
        code += 8;
    } else if(y < ymin){
        code += 4;
    }

    if(x > xmax){
        code += 2;
    }
    else if(x < xmin){
        code += 1;
    }
}

void cohenSutherlandLineClip(double x0, double y0, double x1, double y1, double xmin, double xmax, double ymin, double ymax){
    unsigned char outcode0, outcode1, outcodeOut;
    bool accept = false, done = false;
    double x, y;

    outcode0 = code(x0, y0, xmin, xmax, ymin, ymax);
    outcode1 = code(x1, y1, xmin, xmax, ymin, ymax);

    do{
        if(!outcode0 && !outcode1){ //trivial draw and exit
            accept = true;
            done = true;
        } else if((outcode0 & outcode1) != 0){ // trivial reject and exit
            done = true;
        } else {
            outcodeOut = (outcode0) ? outcode0 : outcode1; // pick an out vertice
            if (outcodeOut & 8){ // discart top
                x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
                y = ymax;
            } else if(outcodeOut & 4){ // discart bottom
                x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
                y = ymin;
            } else if(outcodeOut & 2){
                y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
                x = xmax;
            } else if(outcodeOut & 1){
                y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
                x = xmin;
            }

            if(outcodeOut == outcode0) {
                x0 = x;
                y0 = y;
                outcode0 = code(x0, y0, xmin, xmax, ymin, ymax);
            } else {
                x1 = x;
                y1 = y;
                outcode1 = code(x1, y1, xmin, xmax, ymin, ymax);
            }
        }
    }while (!done);

    if (accept){
        DrawLineReal(x0, y0, x1, y1);
    }
}

int main(){
    return 0;
}