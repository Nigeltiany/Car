import vsync.*;
import ipcapture.*;
import controlP5.*;
import processing.serial.*;

IPCapture camera;
Proximity prox;
ValueReceiver receiver;
ValueReceiver carReceiver;
ValueSender sender;
AnalogMeter speedOMeter;
AnalogMeter revMeter;

//proximity sensor variables
public int left;
public int right;
public int centerLeft;
public int centerRight;
public int rear;

//ANALOG METRICS VARIABLES
public int carSpeed;
public int minSpeed;
public int maxSpeed;
public int command;
public int carGear;
public int gearMaxSpeed;

//COMMAND VARIABLE
public int pCommand;

//MAP AND GPS VARIABLES
PImage map;
long previousUpdate = 0; // last time map was updated
static long interval = 1000; // update after interval milliseconds
ArrayList<String> travelledPath = new ArrayList();
Boolean showTravelPath;
Boolean runOnce;
public String latitude;
public String longitude;
String coordinatesTravelled = "";

void setup(){
  size(1080,730);
  
  //IP CAMERA SETUP
  camera = new IPCapture(this, "http://195.235.198.107:3346/axis-cgi/mjpg/video.cgi?resolution=320x240", "", "");
  camera.start();
  
  //PROXIMITY SENSORS
  prox = new Proximity(0,0,8.8);
  color c = color(117,117,117);
  prox.setAreaColor(c);
  
  //DATA TRANSFER WITH ARDUINO
  Serial serial = new Serial(this, "COM3", 115200);
  receiver = new ValueReceiver(this, serial);
  receiver.observe("left");            //1 //<>//
  receiver.observe("right");           //2
  receiver.observe("centerLeft");      //3
  receiver.observe("centerRight");     //4
  receiver.observe("rear");            //5
  receiver.observe("maxSpeed");        //6
  receiver.observe("minSpeed");        //7
  receiver.observe("carSpeed");        //8      
  receiver.observe("carGear");         //9
  //receiver.observe("gearMaxSpeed");    //2
  
  carReceiver = new ValueReceiver(this, serial);
  //carReceiver.observe("command");         //1
  //carReceiver.observe("gearMaxSpeed");    //2
  carReceiver.observe("minSpeed"); 
  
  //sender = new ValueSender(this, serial); 
  //sender.observe("pCommand");
  
  //ANALOG METRICS
  speedOMeter = new AnalogMeter(width/2,height-130,240,"gauge.png");
  speedOMeter.extraEllipse(true);
  //speedOMeter.setMaxReading(maxSpeed); // max expected data in this case for a 16bit PWM it is 4096
  //speedOMeter.set
  revMeter = new AnalogMeter(width/3,height-110,210,"revmeter.png");
  revMeter.setStartPoint(3.1);
  revMeter.setEndPoint(6.25);
  //revMeter.setMaxReading(1300);
  revMeter.extraEllipse(false);
  
  //MAP AND GPS
  showTravelPath = true;
  runOnce = false;
  //StartUP Image
  map = loadImage("http://files.site-fusion.co.uk/webfusion146840/image/horizontal-logo-transparent-background-2-1024x259.png");
}

void draw(){
  initCamera();
  //drawMap(0,height/2,270,height/2);
  visualizeSensorData();
  visualizeAnalogData();
}

void initCamera(){
  if (camera.isAvailable()) {
    camera.read();
    imageMode(CENTER);
    image(camera,width/2,height/2,540,380);
    //camera.start();
  }
}

void visualizeSensorData(){
  prox.show();
  prox.leftSensor((left==0)? 400 : left);
  prox.rightSensor((right==0)? 400 : right);
  prox.centerLeftSensor((centerLeft==0)? 400 : centerLeft);
  prox.centerRightSensor((centerRight==0)? 400 : centerRight);
  prox.rearSensor((rear==0)? 400 : rear);
}

void visualizeAnalogData(){
  speedOMeter.setMaxReading(maxSpeed);
  revMeter.setMaxReading(1300);
  revMeter.setValue(carSpeed);
  speedOMeter.setValue(carSpeed);
}

void prepareMap(int Width,int Height,float lat,float lng){
  String urlStart = "http://maps.googleapis.com/maps/api/staticmap?";
  String format = "format=png";
  String size = "&size="+String.valueOf(Width)+"x"+String.valueOf(Height);
  String type = "&maptype=roadmap"; //roadmap or terrain or satellite or hybrid
  latitude = String.valueOf(lat);
  longitude = String.valueOf(lng);
  String marker = "&markers=color:red%7Clabel:C%7C"+latitude+","+longitude;
  String API_KEY = "&key=AIza...";
  
  if(showTravelPath == false){
    map = loadImage(urlStart+format+size+type+marker+API_KEY,"png");
  }
    else
  {
    StringBuilder sb = new StringBuilder();
    for(String aPair : travelledPath){
      sb.append(aPair);
    }
    String pathcolor = "color:0xff0000ff|";
    String weight = "weight:5|";
    coordinatesTravelled = sb.toString();
    int index = (coordinatesTravelled.lastIndexOf("|")<0)? 0 : coordinatesTravelled.lastIndexOf("|");
    String wholePath = "&path="+pathcolor+weight+coordinatesTravelled.substring(0,index);
    map = loadImage(urlStart+format+size+type+marker+wholePath+API_KEY,"png");
    //println(urlStart+format+size+type+marker+wholePath+API_KEY);
  }
  
}

void drawMap(float x,float y,float Width,float Height){
  //MAP AND GPS
  long runTime = millis();
  //if(runOnce == true){
  //  //prepareMap(int(Width),int(Height),45.808331,-79.61033400000002);
  //  //runOnce = true;
  //}
  if(runTime - previousUpdate >= interval){
    prepareMap(int(Width),int(Height),45.808331+mouseY/100000.0,79.61033400000002+mouseX/100000.0);
    previousUpdate = runTime;
    //println(previousUpdate/1000);
    travelledPath.add(latitude+","+longitude+"|");
  }
  imageMode(CORNER);
  //DRAW MAP
  image(map,x,y,Width,Height);
}

void carControls(){

}

void keyPressed() {
  if (key == CODED) {
    if (keyCode == UP) {
      pCommand = int('w');
      println("Processing sent " + pCommand);
    } else if (keyCode == DOWN) {
      pCommand = int('s');
      println("Processing sent " + pCommand);
    } 
  }
  if (keyPressed) {
    if (key == 'd' || key == 'D') {
      pCommand = int('d');
      println("Processing sent " + pCommand);
    }
    if (key == 'w' || key == 'W') {
      pCommand = int('w');
      println("Processing sent " + pCommand);
    }
  }
  println(); 
  println("-------------------------------------------"); 
  println("arduino heard " + command);
  println("Speed " + carSpeed);
  println("maxSpeed " + maxSpeed);
  println("Gear " + carGear);
  println("-------------------------------------------"); 
  println("Gear Speed " + gearMaxSpeed);
  println("minSpeed " + minSpeed);
  println("-------------------------------------------"); 
}
