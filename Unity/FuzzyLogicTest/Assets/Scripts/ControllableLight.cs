using UnityEngine;
using System.Collections;

public class ControllableLight : MonoBehaviour {
	public bool Enabled = true;
	Light mLight = null;
	void Start () {
		mLight = (Light)GetComponent ("Light");
	}
	public void UpdateIntensity ( float delta ) {
		if ( Enabled && (!double.IsNaN (delta))) {
			float v = (float)delta;
			mLight.intensity += v;
		}
	}
}
