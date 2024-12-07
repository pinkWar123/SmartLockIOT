// #include "WorkScheduler.h"
// #include "Timer.h"

// /*
// * Chương trình ví dụ 1:
// *
// */

// //khởi tạo các job
// WorkScheduler *printToSerialScheduler;
// WorkScheduler *printToSerial100msScheduler;

// //chúng ta không dùng pin gì hết vì mình đã viết thêm một cập nhập nhỏ
// void printToSerial() {
// 	static unsigned long counter = 0;
// 	counter++;
// 	Serial.print("kLaserCutter - ksp. Lan thu: ");
// 	Serial.println(counter);
// }

// void printToSerial100ms() {
// 	static unsigned long counter = 0;
// 	counter++;
// 	Serial.print("ksp's handsome!!! Lan thu: ");
// 	Serial.println(counter);
// }

// void setup()
// {

// 	//Khởi tạo serial ở mức baudrate 115200
// 	Serial.begin(115200);

// 	//Khởi gạo class timer (design pattern singleton) - bắt buộc phải có trong hàm setup (trước khi khởi tạo các job)
// 	Timer::getInstance()->initialize();

// 	//Khởi tạo một công việc (job) - không đùng đến một pin cụ thể nào đó mà chỉ thực hiện các tác vụ như in serial monitor hoăc đọc các cảm biến có nhiều chân ^_^
// 	printToSerialScheduler = new WorkScheduler(1000UL, printToSerial);

// 	//print ra nhanh hơn
// 	printToSerial100msScheduler = new WorkScheduler(100UL, printToSerial100ms);

// }

// //trong hàm loop chỉ nên có những hàm này, bạn muốn viết một chức năng khác? Xin hãy tạo một job và đưa vào thời khóa biểu scheduler như hàm dưới

// void loop()
// {
// 	//đầu hàm loop phải có để cập nhập thời điểm diễn ra việc kiểm tra lại các tiến trình
// 	Timer::getInstance()->update();

// 	//Không quan trọng thứ tự các job, các job này là các job thực hiện các công việc độc lập với nhau
// 	printToSerialScheduler->update();
// 	printToSerial100msScheduler->update();

// 	//cuối hàm loop phải có để cập nhập lại THỜI ĐIỂM (thời điểm chứ ko phải thời gian nha, tuy tiếng Anh chúng đều là time) để cho lần xử lý sau
// 	Timer::getInstance()->resetTick();

// }