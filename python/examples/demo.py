import os
import sys

src_path = os.path.abspath(os.path.join(os.path.dirname(__file__), '../src'))
if src_path not in sys.path:
    sys.path.insert(0, src_path)

from logger import info, err, get_logger
import time

def run_demo():
    """
    Demonstrates the usage of the asynchronous logger in Python.
    """
    print("Starting Python Logger Demo...")
    
    # Configure file logging
    get_logger().set_file("demo.log")
    
    info("DemoApp", "This is an info message")
    err("DemoApp", "This is an error message")
    
    for i in range(5):
        info("Worker", f"Processing item {i}...")
        time.sleep(0.1)
        
    print("Demo finished. Check demo.log for output.")
    
    # Ensure all logs are flushed before exit
    get_logger().stop()

if __name__ == "__main__":
    run_demo()
