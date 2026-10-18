import pandas as pd
import matplotlib.pyplot as plt
import os
import glob

# Set the path to your CSV files
csv_path = "/home/sepehr/uni/DS/paper/AnchorList_python/timeTakenResults/"

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


def load_and_plot_comparison(test_name, title):
    """Load ELL and list CSV files and create comparison plot"""
    ell_file = f"{csv_path}{test_name}_anchor_results.csv"
    vec_file = f"{csv_path}{test_name}_list_results.csv"
    print(ell_file)
    print(vec_file)
    # Check if both files exist
    if not (os.path.exists(ell_file) and os.path.exists(vec_file)):
        print(f"Skipping {test_name}: Missing files")
        return

    try:
        # Load data
        ell_data = pd.read_csv(ell_file)
        vec_data = pd.read_csv(vec_file)

        print(ell_data.iloc[-1])
        print(vec_data.iloc[-1])
        # Create the plot
        plt.figure(figsize=(12, 8))

        # Plot both datasets
        plt.plot(
            ell_data["Size"],
            ell_data["Time"],
            label="AnchorList",
            marker="o",
            linewidth=2,
            markersize=4,
        )
        plt.plot(
            vec_data["Size"],
            vec_data["Time"],
            label="list",
            marker="s",
            linewidth=2,
            markersize=4,
        )
        plt.xscale("log")

        # Customize the plot
        plt.xlabel("Size", fontsize=12)
        plt.ylabel("Time (seconds)", fontsize=12)
        plt.title(
            f"{title}\nAnchorList vs list Performance (Linear Scale)",
            fontsize=14,
            fontweight="bold",
        )
        plt.legend(fontsize=11)
        plt.grid(True, alpha=0.3)

        # Remove log scale logic

        # Improve layout
        plt.tight_layout()

        # Save the plot
        output_file = f"{csv_path}plots_linear/{test_name}_comparison.png"
        os.makedirs(os.path.dirname(output_file), exist_ok=True)
        plt.savefig(output_file, dpi=300, bbox_inches="tight")
        # plt.show()

        print(f"Saved plot: {output_file}")

    except Exception as e:
        print(f"Error processing {test_name}: {e}")


def create_summary_plot():
    """Create a summary plot showing performance ratios"""
    plt.figure(figsize=(15, 10))

    operation_types = ["Insert", "Remove", "Get"]
    colors = ["#1f77b4", "#ff7f0e", "#2ca02c"]

    for i, op_type in enumerate(operation_types):
        plt.subplot(2, 2, i + 1)

        # Find relevant test categories for this operation type
        relevant_tests = []
        if op_type == "Insert":
            relevant_tests = [
                cat
                for cat in test_categories
                if "push" in cat[0].lower() or "insert" in cat[0].lower()
            ]
        elif op_type == "Remove":
            relevant_tests = [
                cat
                for cat in test_categories
                if "pop" in cat[0].lower() or "remove" in cat[0].lower()
            ]
        else:  # Get
            relevant_tests = [
                cat for cat in test_categories if "get_after" in cat[0].lower()
            ]

        # Plot performance ratios for relevant tests
        for test_name, title in relevant_tests[
            :5
        ]:  # Limit to first 5 to avoid overcrowding
            ell_file = f"{csv_path}{test_name}_anchor_results.csv"
            vec_file = f"{csv_path}{test_name}_list_results.csv"

            if os.path.exists(ell_file) and os.path.exists(vec_file):
                try:
                    ell_data = pd.read_csv(ell_file)
                    vec_data = pd.read_csv(vec_file)

                    # Calculate ratio (list time / ELL time)
                    ratio = vec_data["Time"] / ell_data["Time"]

                    plt.plot(
                        ell_data["Size"],
                        ratio,
                        label=test_name.replace("_", " ").title(),
                        marker="o",
                        linewidth=2,
                        markersize=3,
                    )
                except:
                    continue

        plt.xlabel("Size")
        plt.ylabel("Performance Ratio (list/ELL)")
        plt.title(f"{op_type} Operations \nlist Time / ELL Time")
        plt.legend(fontsize=8)
        plt.grid(True, alpha=0.3)
        plt.axhline(
            y=1, color="red", linestyle="--", alpha=0.7, label="Equal Performance"
        )

    plt.tight_layout()
    output_file = f"{csv_path}plots/performance_summary.png"
    plt.savefig(output_file, dpi=300, bbox_inches="tight")
    # plt.show()
    print(f"Saved summary plot: {output_file}")


# Main execution
if __name__ == "__main__":
    print("Creating comparison plots for AnchorList vs list...")

    # Create individual comparison plots
    for test_name, title in test_categories:
        load_and_plot_comparison(test_name, title)

    # Create summary plot
    create_summary_plot()

    print("All plots have been generated!")
