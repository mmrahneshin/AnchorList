import pandas as pd
import matplotlib.pyplot as plt
import os
import glob

# Set the path to your CSV files
csv_path = "/home/sepehr/uni/DS/paper/AnchorList_JAVA/memoryResults/"

# Define the test categories and their corresponding file patterns
test_categories = [
    ("pushBack", "Push Back Operations"),
    ("popBackAfterPushBack", "Pop Back After Push Back"),
    ("popFrontAfterPushBack", "Pop Front After Push Back"),
    ("removeRandomIndicesAfterPushBack", "Remove Random Indices After Push Back"),
    ("pushFront", "Push Front Operations"),
    ("popFrontAfterPushFront", "Pop Front After Push Front"),
    ("popBackAfterPushFront", "Pop Back After Push Front"),
    ("insertRandomIndices", "Insert Random Indices"),
    ("removeRandomIndices", "Remove Random Indices"),
    ("pushFront_pushBack", "Push Front then Push Back"),
    ("popFront_popBack", "Pop Front then Pop Back"),
    ("pushBack_pushFront", "Push Back then Push Front"),
    ("popBack_popFront", "Pop Back then Pop Front"),
]


def load_and_plot_comparison(test_name, title):
    """Load ELL and ArrayList CSV files and create comparison plot"""
    ell_file = f"{csv_path}{test_name}_ell_memory_results.csv"
    vec_file = f"{csv_path}{test_name}_vec_memory_results.csv"

    # Check if both files exist
    if not (os.path.exists(ell_file) and os.path.exists(vec_file)):
        print(f"Skipping {test_name}: Missing files")
        return

    try:
        # Load data
        ell_data = pd.read_csv(ell_file)
        vec_data = pd.read_csv(vec_file)

        ell_data = ell_data.sort_values(by=["Size"])
        vec_data = vec_data.sort_values(by=["Size"])
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
            label="ArrayList",
            marker="s",
            linewidth=2,
            markersize=4,
        )

        plt.xscale("log")
        plt.yscale("log")

        # Customize the plot
        plt.xlabel("Size", fontsize=12)
        plt.ylabel("Memory (KB)", fontsize=12)
        plt.title(
            f"{title}\nAnchorList vs ArrayList Performance",
            fontsize=14,
            fontweight="bold",
        )
        plt.legend(fontsize=11)
        plt.grid(True, alpha=0.3)

        # Remove log scale logic

        # Improve layout
        plt.tight_layout()

        # Save the plot
        output_file = f"{csv_path}plots/{test_name}_comparison.png"
        os.makedirs(os.path.dirname(output_file), exist_ok=True)
        plt.savefig(output_file, dpi=300, bbox_inches="tight")
        plt.show()

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
            ell_file = f"{csv_path}{test_name}_ell_results.csv"
            vec_file = f"{csv_path}{test_name}_vec_results.csv"

            if os.path.exists(ell_file) and os.path.exists(vec_file):
                try:
                    ell_data = pd.read_csv(ell_file)
                    vec_data = pd.read_csv(vec_file)

                    # Calculate ratio (ArrayList time / ELL time)
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
        plt.ylabel("Performance Ratio (ArrayList/ELL)")
        plt.title(f"{op_type} Operations\nArrayList Time / ELL Time")
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
    print("Creating comparison plots for AnchorList vs ArrayList...")

    # Create individual comparison plots
    for test_name, title in test_categories:
        load_and_plot_comparison(test_name, title)

    # Create summary plot
    create_summary_plot()

    print("All plots have been generated!")
