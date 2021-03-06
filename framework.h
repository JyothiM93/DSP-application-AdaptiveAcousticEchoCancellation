#ifndef framework_h_included
#define framework_h_included

/* Configuration */
#define DSP_SAMPLE_RATE         8000
#define DSP_BLOCK_SIZE          32
#define DSP_INPUT_GAIN          1
#define DSP_OUTPUT_ATTENUATION  0

/* Constants */
#define DSP_FREQUENCY           (12288000*16/2)

/* Framework interface */
typedef struct {
    _Fract left;
    _Fract right;
} sample_t;

void dsp_init(void);
void dsp_start(void);
void dsp_stop(void);
sample_t *dsp_get_audio(void);
unsigned int dsp_get_keys(void);
unsigned int dsp_get_cycles(void);
void dsp_set_leds(unsigned int value);

#endif
