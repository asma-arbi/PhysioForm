import cv2

# Function to perform motion detection
def motion_detection():
    # Open the default camera
    cap = cv2.VideoCapture(0)

    # Check if the camera was successfully opened
    if not cap.isOpened():
        print("Error: Unable to open camera")
        return

    # Initialize variables for motion detection
    _, prev_frame = cap.read()
    prev_gray = cv2.cvtColor(prev_frame, cv2.COLOR_BGR2GRAY)

    # Loop to continuously capture and process frames from the camera
    while True:
        # Capture frame-by-frame
        ret, frame = cap.read()

        # If the frame was not captured successfully, break out of the loop
        if not ret:
            print("Error: Unable to capture frame")
            break

        # Convert the current frame to grayscale
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        # Compute the absolute difference between the current and previous frames
        frame_diff = cv2.absdiff(prev_gray, gray)

        # Apply a threshold to the frame difference to highlight areas of motion
        _, thresh = cv2.threshold(frame_diff, 30, 255, cv2.THRESH_BINARY)

        # Find contours in the thresholded image
        contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        # Draw rectangles around the areas of motion and add text
        for contour in contours:
            if cv2.contourArea(contour) < 1000:
                continue
            x, y, w, h = cv2.boundingRect(contour)
            cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
            cv2.putText(frame, 'Motion detected', (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

        # Display the processed frame
        cv2.imshow('Motion Detection', frame)

        # Update the previous frame
        prev_gray = gray

        # Check for the 'q' key to exit the loop and close the window
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # Release the camera and close the OpenCV window
    cap.release()
    cv2.destroyAllWindows()

# Entry point of the script
if __name__ == "__main__":
    motion_detection()

