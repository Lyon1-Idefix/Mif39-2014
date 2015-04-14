using UnityEngine;
using UnityEngine.Events;

using System;
using System.Collections;

public class LightSensor : MonoBehaviour {
	Camera mCamera;
	Texture2D sensorMapping = null;
	// Use this for initialization
	void Start () {
		mCamera = (Camera)GetComponent ("Camera");
	}
	
	// Update is called once per frame
	void Update () {
		RenderTexture cameraRT = mCamera.targetTexture;
		if (cameraRT == null)
			return;
		RenderTexture currentRT = RenderTexture.active;
		RenderTexture.active = cameraRT;
		if ( sensorMapping == null )
			sensorMapping = new Texture2D (1, 1);
		sensorMapping.ReadPixels (new Rect (0, 0, cameraRT.width, cameraRT.height), 0, 0);
		sensorMapping.Apply();
		RenderTexture.active = currentRT;
		Color pixel = sensorMapping.GetPixel (0, 0);
		float gray = pixel.grayscale;
		OnValueChanged.Invoke (gray);
	}
	public SensorEvent OnValueChanged;
	[Serializable]
	public class SensorEvent : UnityEvent<float>
	{
	}
}
