import cv2
import time
import os

# Load the pre-trained face detection cascade
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

# Function to perform face recognition
def recognize_faces():
    # Open the default camera
    cap = cv2.VideoCapture(0)

    # Initialize the font for displaying text
    font = cv2.FONT_HERSHEY_SIMPLEX

    # List of reference images
    reference_images = ['C:/Users/malek/Desktop/integration/photoMalek.jpg',
    'C:/Users/malek/Desktop/integration/photoAli.jpg',
    'C:/Users/malek/Desktop/integration/photoHalima.jpg',
    'C:/Users/malek/Desktop/integration/photoAsma.jpg',
    'C:/Users/malek/Desktop/integration/photoFeriel.jpg',
     'C:/Users/malek/Desktop/integration/photoAbdoo.jpg']  # Add your reference image paths here

    # Load and preprocess reference images
    reference_faces = []
    for img_path in reference_images:
        reference_image = cv2.imread(img_path)
        if reference_image is None or reference_image.size == 0:
            print(f"Error loading reference image: {img_path}")
            continue

        reference_gray = cv2.cvtColor(reference_image, cv2.COLOR_BGR2GRAY)
        faces = face_cascade.detectMultiScale(reference_gray, scaleFactor=1.3, minNeighbors=5)

        if len(faces) > 0:
            (x_ref, y_ref, w_ref, h_ref) = faces[0]
            reference_face = reference_gray[y_ref:y_ref+h_ref, x_ref:x_ref+w_ref]
            reference_faces.append((reference_face, img_path))  # Store both the face and its corresponding image path

    if len(reference_faces) == 0:
        print("No valid face detected in reference images.")
        return ""

    # Define start_time
    start_time = time.time()

    while True:
        # Read a frame from the camera
        ret, frame = cap.read()
        
        # Convert the frame to grayscale
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        
        faces = face_cascade.detectMultiScale(gray, scaleFactor=1.3, minNeighbors=5)

        for (x, y, w, h) in faces:
            cv2.rectangle(frame, (x, y), (x+w, y+h), (255, 0, 0), 2)
            
            detected_face = gray[y:y+h, x:x+w]
            detected_face_resized = cv2.resize(detected_face, (w_ref, h_ref))

            matching_image_path = ""
            for (idx, (reference_face, img_path)) in enumerate(reference_faces):
                correlation = cv2.matchTemplate(detected_face_resized, reference_face, cv2.TM_CCOEFF_NORMED)[0][0]
                
                if correlation > 0.30:  # Adjust the threshold as needed
                    matching_image_path = img_path
                    break
            
            if matching_image_path:
                cv2.putText(frame, 'Match Found', (x, y-10), font, 0.9, (0, 255, 0), 2, cv2.LINE_AA)
            else:
                cv2.putText(frame, 'No Match', (x, y-10), font, 0.9, (0, 0, 255), 2, cv2.LINE_AA)

        cv2.imshow('Face Recognition', frame)

        # Check if 6 seconds have passed
        if time.time() - start_time > 6:
            break

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

    return matching_image_path if matching_image_path else ""

result = recognize_faces()
print(result )
