import os
import sys

src_path = os.path.abspath(os.path.join(os.path.dirname(__file__), '../src'))
if src_path not in sys.path:
    sys.path.insert(0, src_path)


from logger import Logger, get_logger, info, err
import time
import os

def test_logger():
    log_path = "test_python.log"
    
    # Test Singleton
    l1 = Logger()
    l2 = get_logger()
    assert l1 is l2
    
    # Test console and file logging
    l1.set_file(log_path)
    
    info("TestSender", "Testing info message")
    err("TestSender", "Testing error message")
    
    # Give some time for async processing
    time.sleep(0.5)
    
    assert os.path.exists(log_path)
    with open(log_path, 'r') as f:
        content = f.read()
        assert "Testing info message" in content
        assert "Testing error message" in content
    
    # Cleanup
    l1.stop()
    if os.path.exists(log_path):
        os.remove(log_path)
    
    print("Python logger tests passed!")

if __name__ == "__main__":
    test_logger()
