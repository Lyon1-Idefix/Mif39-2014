using UnityEngine;
using UnityEngine.UI;
using UnityEngine.Events;

using System;
using System.Collections;

[Serializable]
public struct SimpleDimmerLightPanel {
	public Light controlledLight;
	public Toggle lightAutoControlToggle;
	public Slider lightIntensityControl;
	public Text lightIntensityValue;
	public void Update () {
		lightIntensityControl.value = controlledLight.intensity;
		lightIntensityValue.text = string.Format ("{0:0.000}", controlledLight.intensity);
	}
}
[Serializable]
public struct SimpleDimmerSensorPanel {
	public Slider requiredIntensityControl;
	public Text requiredIntensityValue;
}

public class ModalPanel : MonoBehaviour {
	public SimpleDimmer controller;
	public SimpleDimmerSensorPanel sensorDimmer;
	public SimpleDimmerLightPanel firstLightDimmer;
	public SimpleDimmerLightPanel secondLightDimmer;
	//
	private static ModalPanel modalPanel;
	public static ModalPanel Instance () {
		if (!modalPanel) {
			modalPanel = FindObjectOfType (typeof(ModalPanel)) as ModalPanel;
			if (!modalPanel)
				Debug.LogError ("There needs to be one active ModalPanel script on a GameObject in your scene.");
		}
		return modalPanel;
	}
	//
	void Update () {
		firstLightDimmer.Update ();
		secondLightDimmer.Update ();
	}
	public void setFirstLightAutoMode ( bool value ) {
		ControllableLight clight = (ControllableLight)firstLightDimmer.controlledLight.gameObject.GetComponent (typeof(ControllableLight));
		clight.Enabled = value;
		firstLightDimmer.lightIntensityControl.interactable = !value;
	}
	public void setFirstLightIntensityFromSlider ( float value ) {
		firstLightDimmer.controlledLight.intensity = value;
		firstLightDimmer.lightIntensityValue.text = string.Format ("{0:0.000}", value);
	}
	public void setSecondLightAutoMode ( bool value ) {
		ControllableLight clight = (ControllableLight)secondLightDimmer.controlledLight.gameObject.GetComponent (typeof(ControllableLight));
		clight.Enabled = value;
		secondLightDimmer.lightIntensityControl.interactable = !value;
	}
	public void setSecondLightIntensityFromSlider ( float value ) {
		secondLightDimmer.controlledLight.intensity = value;
		secondLightDimmer.lightIntensityValue.text = string.Format ("{0:0.000}", value);
	}
	public void requiredLightingChanged (float value) {
		controller.requiredLightingChanged (value);
		sensorDimmer.requiredIntensityValue.text = string.Format ("{0:0.000}", value);
	}
}
