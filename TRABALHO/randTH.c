unsigned char temp[25] = 	{22, 21, 25, 22, 25, 23, 28, 20, 25, 27, 22, 22, 28, 22, 29, 28, 27, 27, 26, 23, 28, 22, 21, 27, 29};
unsigned char hum[25] = 	{63, 70, 60, 60, 68, 67, 66, 60, 67, 60, 63, 66, 67, 62, 69, 66, 64, 63, 70, 69, 67, 65, 67, 62, 70};

char randTemp() {
    static char i = 0; 
    char result = temp[i]; 
    i++; 
    if (i > 24) {
        i = 0;
    }
    return result;
}

char randHum() {
    static char j = 0; 
    char result = temp[j]; 
    j++; 
    if (j > 24) {
        j = 0;
    }
    return result;
}