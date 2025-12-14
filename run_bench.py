import subprocess
import os
import sys

def main():
    # Path Configuration
    project_root = os.path.dirname(os.path.abspath(__file__))
    
    # Define paths
    build_dir = os.path.join(project_root, "build")
    executable = os.path.join(build_dir, "rmi_bench")
    data_dir = os.path.join(project_root, "data")

    # Validation 
    if not os.path.exists(executable):
        print(f"Error: Executable not found at {executable}")
        sys.exit(1)

    if not os.path.exists(data_dir):
        print(f"Error: Data directory not found at {data_dir}")
        sys.exit(1)

    # Experiment Settings 
    datasets = [f for f in os.listdir(data_dir) if f.endswith("uint64")]
    
    if not datasets:
        print(f"Error: No datasets found in {data_dir}")
        sys.exit(1)

    # Model sizes to benchmark (Stage 2 size)
    model_counts = [100, 1000, 10000, 100000, 1000000]

    # Execution Loop 
    print(f"{'Dataset':<25} | {'Models':<8} | {'MaxError':<10} | {'RMI (ns)':<10} | {'BS (ns)':<10} | {'Speedup':<8}")
    print("-" * 85)

    for dataset_name in datasets:
        dataset_path = os.path.join(data_dir, dataset_name)
        
        for models in model_counts:
            try:
                # Run the C++ executable: ./rmi_bench <data_path> <num_models>
                cmd = [executable, dataset_path, str(models)]
                
                # Capture output
                result = subprocess.run(cmd, capture_output=True, text=True)
                
                if result.returncode != 0:
                    print(f"Error running {dataset_name} with {models} models.")
                    print(result.stderr)
                    continue

                # Parse CSV Output: result, filename, models, max_error, rmi_time, bs_time
                for line in result.stdout.split('\n'):
                    if line.startswith("RESULT"):
                        parts = line.split(',')
                        
                        # Extract metrics
                        max_err = parts[3]
                        rmi_ns = float(parts[4])
                        bs_ns = float(parts[5])
                        
                        # Calculate speedup (Baseline / RMI)
                        speedup = bs_ns / rmi_ns if rmi_ns > 0 else 0.0
                        
                        print(f"{dataset_name:<25} | {models:<8} | {max_err:<10} | {rmi_ns:<10.2f} | {bs_ns:<10.2f} | {speedup:<8.2f}x")

            except Exception as e:
                print(f"Exception running {dataset_name}: {e}")

if __name__ == "__main__":
    main()