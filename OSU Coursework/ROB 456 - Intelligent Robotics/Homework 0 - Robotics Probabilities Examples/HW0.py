import numpy
import scipy
import matplotlib.pyplot as pyplot
import time

numpy.random.seed(int(time.time()))

PART_1_COEFFICIENTS = numpy.array([-0.1, 4.0, -0.1, 10.0], float)
PART_1_X_LIMITS = [-10.0, 25.0]


def plot_form(axis_handle, x_limit=None, title="", x_label="x", y_label="f(x)"):
    if x_limit is not None:
        axis_handle.set_xlim(x_limit)

    axis_handle.set_title(title)
    axis_handle.set_xlabel(x_label)
    axis_handle.set_ylabel(y_label)


def part_1_polynomial(x_input):
    return numpy.polyval(PART_1_COEFFICIENTS, x_input)


def part_2_plot():
    x_limit_min = PART_1_X_LIMITS[0]
    x_limit_max = PART_1_X_LIMITS[1]

    temp = numpy.linspace(x_limit_min, x_limit_max, 351, dtype=float)

    function_handle_1, axis_handle_1 = pyplot.subplots()
    axis_handle_1.plot(temp, part_1_polynomial(temp), "b-")

    plot_form(axis_handle_1, PART_1_X_LIMITS, "Original Polynomial")
    function_handle_1.savefig("figures/hw0_original_polynomial.pdf", bbox_inches="tight")


def part_3():
    x_limit_min = PART_1_X_LIMITS[0]
    x_limit_max = PART_1_X_LIMITS[1]

    bin_width = (x_limit_max-x_limit_min) / 14.0
    x_bin = numpy.arange(x_limit_min, x_limit_max, bin_width, float)
    y_bin = part_1_polynomial(x_bin)

    function_handle, axis_handle = pyplot.subplots()
    axis_handle.bar(x_bin + bin_width/2.0, y_bin, width=bin_width, edgecolor="k")

    plot_form(axis_handle, PART_1_X_LIMITS, "Discretized Bins")
    function_handle.savefig("figures/hw0_discretized_bins.pdf", bbox_inches="tight")


def part_4():
    x_limit_min = PART_1_X_LIMITS[0]
    x_limit_max = PART_1_X_LIMITS[1]

    bin_width = (x_limit_max - x_limit_min) / 14.0
    x_bin = numpy.arange(x_limit_min, x_limit_max, bin_width, float)
    y_bin = part_1_polynomial(x_bin)

    y_bin_normalized = y_bin / y_bin.sum()

    function_handle, axis_handle = pyplot.subplots()
    axis_handle.bar(x_bin + bin_width / 2.0, y_bin_normalized, width=bin_width, edgecolor="k")

    plot_form(axis_handle, PART_1_X_LIMITS, "Discretized Bins (Normalized) sum=%s" % y_bin_normalized.sum(), y_label="p(k)")
    function_handle.savefig("figures/hw0_discretized_bins_normalized.pdf", bbox_inches="tight")


def part_5_1():
    num_samples = 500
    x_rand_values = numpy.arange(1, num_samples+1, 1, int)
    y_rand_values = numpy.random.random(num_samples)

    function_handle, axis_handle = pyplot.subplots()
    pyplot.plot(x_rand_values, y_rand_values, "k+")
    plot_form(axis_handle, x_limit=[1, num_samples], title="%s Samples, Uniformly Distributed" % num_samples)

    function_handle.savefig("figures/hw0_%s_random_samples.pdf" % num_samples, bbox_inches="tight")
    return (x_rand_values, y_rand_values)


def part_5_2(vals):
    num_samples = 500

    x_limit_min = PART_1_X_LIMITS[0]
    x_limit_max = PART_1_X_LIMITS[1]

    bin_width = (x_limit_max - x_limit_min) / 14.0
    x_bin = numpy.arange(x_limit_min, x_limit_max, bin_width, float)

    x_rand_values = vals[0]
    y_rand_values = vals[1]

    y_random_scaled = y_rand_values * ((x_limit_max - x_limit_min) + x_limit_min)

    function_handle, axis_handle = pyplot.subplots()
    pyplot.plot(x_rand_values, y_random_scaled, "k+")

    for i in range(0, len(x_bin)):
        axis_handle.plot([1, num_samples], [x_bin[0], x_bin[1]])

    plot_form(axis_handle, [1, num_samples], "Random Samples Mapped to X Ranges Of Bins")
    function_handle.savefig("figures/hw0_random_bins_to_ranges.pdf", bbox_inches="tight")


def part_5_3():
    y_count_incorrect = numpy.zeros(x_bin.shape)
    for i in range(0, len(y_rand_scaled)):
        for j in range(len(x_bin), 0, -1):
            if y_rand_scaled[i] > x_bin[j-1]:
                y_count_incorrect[j-1] += 1
                break

    function_handle, axis_handle = pyplot.subplots()
    pyplot.plot(x_bin+b_width/2.0, y_random_incorrect, "k+")
    plot_form(axis_handle, PART_1_X_LIMITS, "Samples per bin (incorrect)", bbox_inches="tight")
    #savefig "hw0_samples_per_bin_incorrect.pdf"

def part_5_4():
    y_bin_cdf = y_bin_normalized.copy()

    i = 0

    while i < len(y_bin_cdf) - 1:
        i += 1
        y_bin_cdf[i] += y_bin_cdf[i-1]

    function_handle, axis_handle = pyplot.subplots()
    axis_handle.plot(x_rand, y_rand, "k+")

    for i in range(0, len(y_bin_cdf)):
        axis_handle.plot([1, num_samples], [y_bin_cdf[0], y_bin_cdf[1]])

    axis_handle.set_title("Dividing up the samples according to bin height")
    function_handle.savefig("hw0_correct_sample_division.pdf", bbox_inches="tight")

    y_count_correct = numpy.zeros(x_bin.shape)

    for i in range(0, len(y_rand)):
        for j in range(len_bin_cdf):
            if y_rand[i] < y_bin_cdf[j]:
                y_count_correct[j] += 1
                break

    function_handle_1, axis_handle_1 = pyplot.subplots()
    axis_handle_1.bar(x_bin + b_width/2.0, y_count_correct, width=b_width, edgecolor="k")
    plot_form(axis_handle_1, x_limit=PART_1_X_LIMITS, "Samples per bin (correct)", y_label="samples")
    function_handle.savefig("hw0_samples_per_bin_correct.pdf", bbox_inches="tight")

def real_part_2():
    pass

if __name__ == '__main__':
    # part_2_plot()
    # part_3()
    # part_4()
    # vals = part_5_1()
    # part_5_2(vals)
    real_part_2()

    pyplot.show()
