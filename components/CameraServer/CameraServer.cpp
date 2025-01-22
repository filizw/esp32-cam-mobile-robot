#include "CameraServer.h"

static const char *CAMERA_TAG{"Camera"}; // Camera tag identifier

//#define AI_THINKER // Define if using Ai Thinker board
#ifdef AI_THINKER
// Ai Thinker PIN Map    
#define CAM_PIN_PWDN    32
#define CAM_PIN_RESET   -1
#define CAM_PIN_XCLK     0
#define CAM_PIN_SIOD    26
#define CAM_PIN_SIOC    27

#define CAM_PIN_D7      35
#define CAM_PIN_D6      34
#define CAM_PIN_D5      39
#define CAM_PIN_D4      36
#define CAM_PIN_D3      21
#define CAM_PIN_D2      19
#define CAM_PIN_D1      18
#define CAM_PIN_D0       5
#define CAM_PIN_VSYNC   25
#define CAM_PIN_HREF    23
#define CAM_PIN_PCLK    22
#else
// WROVER-KIT PIN Map
#define CAM_PIN_PWDN    -1
#define CAM_PIN_RESET   -1
#define CAM_PIN_XCLK    21
#define CAM_PIN_SIOD    26
#define CAM_PIN_SIOC    27

#define CAM_PIN_D7      35
#define CAM_PIN_D6      34
#define CAM_PIN_D5      39
#define CAM_PIN_D4      36
#define CAM_PIN_D3      19
#define CAM_PIN_D2      18
#define CAM_PIN_D1       5
#define CAM_PIN_D0       4
#define CAM_PIN_VSYNC   25
#define CAM_PIN_HREF    23
#define CAM_PIN_PCLK    22
#endif

#define CONFIG_XCLK_FREQ 20000000

#define PART_BOUNDARY "123456789000000000000987654321"
static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

CameraServer::CameraServer()
{
    setTag("CameraServer");
}

CameraServer::CameraServer(uint16_t port) : HTTPServer(port)
{
    setTag("CameraServer");
}

esp_err_t CameraServer::start()
{
    ESP_RETURN_ON_ERROR(cameraInit(), CAMERA_TAG, "failed to initialize the camera");

    sensor_t *s = esp_camera_sensor_get();
    if (s != NULL) {
        s->set_vflip(s, 1);     // Flip camera view vertically
        s->set_hmirror(s, 1);   // Flip camera view horizontally
    }

    esp_err_t err = startServer();
    if(err == ESP_OK)
        err = registerURIHandler("/stream", HTTP_GET, jpgStreamHandler);
    
    return err;
}

esp_err_t CameraServer::cameraInit()
{
    camera_config_t camera_config = {
        .pin_pwdn  = CAM_PIN_PWDN,
        .pin_reset = CAM_PIN_RESET,
        .pin_xclk = CAM_PIN_XCLK,
        .pin_sccb_sda = CAM_PIN_SIOD,
        .pin_sccb_scl = CAM_PIN_SIOC,

        .pin_d7 = CAM_PIN_D7,
        .pin_d6 = CAM_PIN_D6,
        .pin_d5 = CAM_PIN_D5,
        .pin_d4 = CAM_PIN_D4,
        .pin_d3 = CAM_PIN_D3,
        .pin_d2 = CAM_PIN_D2,
        .pin_d1 = CAM_PIN_D1,
        .pin_d0 = CAM_PIN_D0,
        .pin_vsync = CAM_PIN_VSYNC,
        .pin_href = CAM_PIN_HREF,
        .pin_pclk = CAM_PIN_PCLK,

        .xclk_freq_hz = CONFIG_XCLK_FREQ,
        .ledc_timer = LEDC_TIMER_0,
        .ledc_channel = LEDC_CHANNEL_0,

        .pixel_format = PIXFORMAT_JPEG,
        .frame_size = FRAMESIZE_VGA,

        .jpeg_quality = 10,
        .fb_count = 1,
        .fb_location{},
        .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
        .sccb_i2c_port{},
    };

    return esp_camera_init(&camera_config);
}

esp_err_t CameraServer::jpgStreamHandler(httpd_req_t *req)
{
    camera_fb_t *fb{};
    esp_err_t res{};
    size_t _jpg_buf_len{};
    uint8_t *_jpg_buf{};
    char *part_buf[64]{};

    res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
    if(res != ESP_OK)
        return res;

    while(true){
        fb = esp_camera_fb_get();
        if(!fb) {
            ESP_LOGE(CAMERA_TAG, "capture failed");
            res = ESP_FAIL;
            break;
        }

        if(fb->format != PIXFORMAT_JPEG) {
            bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
            if(!jpeg_converted){
                ESP_LOGE(CAMERA_TAG, "JPEG compression failed");
                esp_camera_fb_return(fb);
                res = ESP_FAIL;
            }
        }
        else {
            _jpg_buf_len = fb->len;
            _jpg_buf = fb->buf;
        }

        if(res == ESP_OK)
            res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
        
        if(res == ESP_OK) {
            size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
            res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
        }

        if(res == ESP_OK)
            res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
        
        if(fb->format != PIXFORMAT_JPEG)
            free(_jpg_buf);
        
        esp_camera_fb_return(fb);
        if(res != ESP_OK)
            break;
    }

    return res;
}
