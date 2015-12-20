/*
 * SpectrumTransformer.h
 *
 * Created on: Jul 30, 2015
 *     Author: dpayne
 */

#ifndef _SPECTRUM_TRANSFORMER_H
#define _SPECTRUM_TRANSFORMER_H

#include <fftw3.h>
#include "Transformer/GenericTransformer.h"
#include "Writer/NcursesWriter.h"
#include "Domain/Settings.h"

namespace vis
{

class SpectrumTransformer : public GenericTransformer
{
  public:
    explicit SpectrumTransformer(const Settings *const settings);

    ~SpectrumTransformer() override;

    void execute_mono(pcm_stereo_sample *buffer,
                      vis::NcursesWriter *writer) override;
    void execute_stereo(pcm_stereo_sample *buffer,
                        vis::NcursesWriter *writer) override;

  private:
    void execute(pcm_stereo_sample *buffer,
                        vis::NcursesWriter *writer, const bool is_stereo);
    const Settings *const m_settings;

    /** --- BEGIN MEMBER VARIABLES --- */

    /** --- BEGIN --- fft calculations vars **/
    size_t m_fftw_results;

    double *m_fftw_input_left;
    double *m_fftw_input_right;

    fftw_complex *m_fftw_output_left;
    fftw_complex *m_fftw_output_right;

    fftw_plan m_fftw_plan_left;
    fftw_plan m_fftw_plan_right;
    /** --- END --- fft calculations vars **/

    /** --- BEGIN --- frequency cutoff calculations vars **/
    std::vector<uint32_t> m_low_cutoff_frequencies;

    std::vector<uint32_t> m_high_cutoff_frequencies;

    std::vector<double> m_frequency_constants_per_bin;

    int32_t m_previous_win_width; // Used to determine if freq cutoffs need to
                                  // be re-calculated
    /** --- END --- frequency cutoff calculations vars **/

    uint64_t m_silent_runs; // Used to determine if the transformer should sleep
                            // and wait for input

    // Holds the current bar heights after processing, this is done as a member
    // function to avoid memory allocations on every run
    std::vector<double> m_bars_left;
    std::vector<double> m_bars_right;

    // falloff vectors hold the previous runs values, this is used to to apply
    // falloff effect for the current run
    std::vector<double> m_bars_falloff_left;
    std::vector<double> m_bars_falloff_right;

    std::vector<double> m_previous_max_heights;

    //Used by monstercat smoothing to apply weights to a bar's height as determined by it's frequency range
    //Note: this is only re-computed when screen width changes
    std::vector<double> m_monstercat_smoothing_weights;

    // Pre-compute colors calculations to avoid duplicate work
    //Note: this is only re-computed when screen height changes
    std::vector<vis::ColorIndex> m_precomputed_colors;

    /** --- END MEMBER VARIABLES --- */

    /** --- BEGIN MEMBER FUNCTIONS --- */

    /**
     * Copies the channel given by "channel_mode" to the fftw_input buffer
     */
    bool prepare_fft_input(pcm_stereo_sample *buffer, uint32_t sample_size,
                           double *fftw_input, ChannelMode channel_mode);

    /**
     * Populates "bars" and "bars_falloff" with the bar heights to be displayed
     */
    virtual void
    create_spectrum_bars(fftw_complex *fftw_output, const size_t fftw_results,
                         const int32_t win_height, const int32_t win_width,
                         const uint32_t number_of_bars,
                         std::vector<double> &bars, std::vector<double> &bars_falloff
                         );

    void
    generate_bars(
    std::vector<double> &bars,
            const uint32_t number_of_bars,
                  const fftw_complex *fftw_output, const size_t fftw_results,
                  const std::vector<uint32_t> &low_cutoff_frequencies,
                  const std::vector<uint32_t> &high_cutoff_frequencies) const;

    void recalculate_cutoff_frequencies(
        uint32_t number_of_bars, std::vector<uint32_t> *low_cutoff_frequencies,
        std::vector<uint32_t> *high_cutoff_frequencies,
        std::vector<double> *freqconst_per_bin);

    void draw_bars(const std::vector<double> &bars, const std::vector<double> &bars_falloff, int32_t win_height,
                   const bool flipped, const std::wstring &bar_row_msg,
                   vis::NcursesWriter *writer);

    void smooth_bars(std::vector<double> &bars);

    std::vector<double> apply_falloff(const std::vector<double> &bars,
                                      std::vector<double> &previous_bars) const;

    void calculate_moving_average_and_std_dev(
        const double new_value, const size_t max_number_of_elements,
        std::vector<double> &old_values, double *moving_average,
        double *std_dev) const;

    void maybe_reset_scaling_window(const double current_max_height,
                                    const size_t max_number_of_elements,
                                    std::vector<double> *values,
                                    double *moving_average, double *std_dev);

    void scale_bars(std::vector<double> &bars, const int32_t height);

    std::wstring create_bar_row_msg(const wchar_t character, uint32_t bar_width);

    void sgs_smoothing(std::vector<double> &bars);
    void monstercat_smoothing(std::vector<double> &bars);

    /**
     * Write's the msg to the writer
     */
    void write(const int32_t row, const int32_t column,
               const vis::ColorIndex color, const std::wstring &msg,
               vis::NcursesWriter *writer);
    /** --- END MEMBER FUNCTIONS --- */
};
}

#endif
