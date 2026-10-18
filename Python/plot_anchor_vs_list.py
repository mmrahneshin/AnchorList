"""Plot Python AnchorList and list benchmark results on log-log axes."""

import os

import matplotlib.pyplot as plt
import pandas as pd
from matplotlib.lines import Line2D

# Results live next to this script.
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
csv_path = os.path.join(SCRIPT_DIR, "timeTakenResults")

# (file suffix, legend label, marker, linestyle)
ANCHOR = ("anchor", "AnchorList", "o", "-")
BASELINES = [
    ("list", "list", "s", "--"),
]
IMPLEMENTATIONS = [ANCHOR] + BASELINES

YLABEL = "Average time per operation (seconds)"

# Define the test categories and their corresponding file patterns
test_categories = [
    ("push_back", "push_back"),
    ("get_after_push_back", "Get"),
    ("pop_back_after_push_back", "pop_back"),
    ("get_after_pop_back_after_push_back", "Get"),
    ("pop_front_after_push_back", "pop_front"),
    ("get_after_pop_front_after_push_back", "Get"),
    ("remove_random_after_push_back", "Remove from random positions"),
    ("get_after_remove_random_after_push_back", "Get"),
    ("push_front", "push_front"),
    ("get_after_push_front", "Get"),
    ("pop_front_after_push_front", "pop_front"),
    ("get_after_pop_front_after_push_front", "Get"),
    ("pop_back_after_push_front", "pop_back"),
    ("get_after_pop_back_after_push_front", "Get"),
    ("insert_random", "Insert at random positions"),
    ("get_after_insert_random", "Get"),
    ("remove_random", "Remove from random positions"),
    ("get_after_remove_random", "Get"),
    ("push_front_push_back", "push_front"),
    ("get_after_push_front_push_back", "Get"),
    ("pop_front_pop_back", "pop_front"),
    ("get_after_pop_front_pop_back", "Get"),
    ("push_back_push_front", "push_back"),
    ("get_after_push_back_push_front", "Get"),
    ("pop_back_pop_front", "pop_back"),
    ("get_after_pop_back_pop_front", "Get"),
]


def load_series(test_name):
    """Return {suffix: DataFrame} for every implementation that has a CSV."""
    series = {}
    for suffix, _, _, _ in IMPLEMENTATIONS:
        path = os.path.join(csv_path, f"{test_name}_{suffix}_results.csv")
        if os.path.exists(path):
            series[suffix] = (
                pd.read_csv(path)
                .sort_values("Size", kind="stable")
                .reset_index(drop=True)
            )
    return series


def average_time(data):
    """Return the average time for one operation in each benchmark size."""
    return data["Time"] / data["Size"]


def load_and_plot_comparison(test_name, title):
    """Load Python result CSVs and create a log-log comparison plot."""
    series = load_series(test_name)
    if not series:
        print(f"Skipping {test_name}: no result files in {csv_path}")
        return

    missing = [label for suffix, label, _, _ in IMPLEMENTATIONS if suffix not in series]
    if missing:
        print(f"Note: {test_name} is missing {', '.join(missing)}")

    try:
        plt.figure(figsize=(12, 8))

        for suffix, label, marker, linestyle in IMPLEMENTATIONS:
            data = series.get(suffix)
            if data is None:
                continue
            plt.plot(
                data["Size"],
                average_time(data),
                label=label,
                marker=marker,
                linestyle=linestyle,
                linewidth=2,
                markersize=4,
            )

        plt.xscale("log")
        plt.yscale("log")

        plt.xlabel("Size", fontsize=12)
        plt.ylabel(YLABEL, fontsize=12)
        plt.title(
            f"{title}\nAnchorList vs list (Logarithmic Scale)",
            fontsize=14,
            fontweight="bold",
        )
        plt.legend(fontsize=11)
        plt.grid(True, alpha=0.3, which="both")

        plt.tight_layout()

        output_file = os.path.join(csv_path, "plots", f"{test_name}_comparison.png")
        os.makedirs(os.path.dirname(output_file), exist_ok=True)
        plt.savefig(output_file, dpi=300, bbox_inches="tight")

        print(f"Saved plot: {output_file}")

    except Exception as e:
        print(f"Error processing {test_name}: {e}")
    finally:
        plt.close()


def relevant_tests_for(op_type):
    """The test categories belonging to one operation family."""
    if op_type == "Insert":
        keys = ("push", "insert")
    elif op_type == "Remove":
        keys = ("pop", "remove")
    else:  # Get
        keys = ("get_after",)
    return [cat for cat in test_categories if any(k in cat[0].lower() for k in keys)]


def create_summary_plot():
    """Performance ratio of list against AnchorList, per operation."""
    operation_types = ["Insert", "Remove", "Get"]
    fig, axes = plt.subplots(1, len(operation_types), figsize=(20, 6))

    for ax, op_type in zip(axes, operation_types):
        tests = relevant_tests_for(op_type)[:5]  # limit to avoid overcrowding
        plotted = []

        for i, (test_name, _) in enumerate(tests):
            series = load_series(test_name)
            anchor_data = series.get("anchor")
            if anchor_data is None:
                continue

            color = plt.cm.tab10(i % 10)
            list_data = series.get("list")
            if list_data is None:
                continue
            merged = pd.merge(
                anchor_data, list_data, on="Size", suffixes=("_anchor", "_list")
            )
            ratio = merged["Time_list"] / merged["Time_anchor"]
            ax.plot(
                merged["Size"],
                ratio,
                color=color,
                linestyle="--",
                marker="o",
                markersize=3,
                linewidth=2,
                label=test_name.replace("_", " ").title(),
            )
            plotted.append(test_name.replace("_", " ").title())

        ax.axhline(y=1, color="red", linestyle=":", alpha=0.7)
        ax.set_xscale("log")
        ax.set_xlabel("Size")
        ax.set_ylabel("Performance ratio (list / AnchorList)")
        ax.set_title(f"{op_type} operations")
        ax.grid(True, alpha=0.3, which="both")

        # Two legends: colour encodes the test, linestyle the implementation.
        color_handles = [
            Line2D([], [], color=plt.cm.tab10(i % 10), marker="o", label=name)
            for i, name in enumerate(plotted)
        ]
        color_legend = ax.legend(handles=color_handles, fontsize=7, loc="upper left")
        ax.add_artist(color_legend)
        style_handles = [
            Line2D([], [], color="0.3", linestyle=ls, label=label)
            for _, label, _, ls in BASELINES
        ]
        style_handles.append(
            Line2D([], [], color="red", linestyle=":", label="Equal cost")
        )
        ax.legend(handles=style_handles, fontsize=7, loc="upper right")

    fig.suptitle(
        "Performance ratio vs AnchorList (below 1 = AnchorList is slower)",
        fontsize=14,
        fontweight="bold",
    )
    fig.tight_layout()

    output_file = os.path.join(csv_path, "plots", "performance_summary.png")
    os.makedirs(os.path.dirname(output_file), exist_ok=True)
    fig.savefig(output_file, dpi=300, bbox_inches="tight")
    plt.close(fig)
    print(f"Saved summary plot: {output_file}")


# Main execution
if __name__ == "__main__":
    print("Creating comparison plots for AnchorList vs list...")

    if not os.path.isdir(csv_path):
        raise SystemExit(f"Results directory not found: {csv_path}")

    # Create individual comparison plots
    for test_name, title in test_categories:
        load_and_plot_comparison(test_name, title)

    # Create summary plot
    create_summary_plot()

    print("All plots have been generated!")
