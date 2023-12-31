import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import minervous.streamdeck

ApplicationWindow {
	id: root

	property Window childWindow

	minimumWidth: Math.max(content.implicitWidth + 40, 500)
	minimumHeight: Math.max(content.implicitHeight + 20, 300)
	visible: true
	title: qsTr('Example Project')

	function deviceTypeToString(type)
	{
		switch(type) {
		case StreamDeck.Original:
			return 'Stream Deck Original'
		case StreamDeck.OriginalV2:
			return 'Stream Deck OriginalV2'
		case StreamDeck.MK2:
			return 'Stream Deck MK2'
		case StreamDeck.Mini:
			return 'Stream Deck Mini'
		case StreamDeck.MiniMK2:
			return 'Stream Deck MiniMK2'
		case StreamDeck.XL:
			return 'Stream Deck XL'
		case StreamDeck.XLV2:
			return 'Stream Deck XLV2'
		case StreamDeck.Pedal:
			return 'Stream Deck Pedal'
		case StreamDeck.Plus:
			return 'Stream Deck +'
		case StreamDeck.Any:
			return 'Any Stream Deck'
		case StreamDeck.Unknown:
		default:
			return 'Unknown device / diconnected'
		}
	}

	onClosing: {
		childWindow?.close()
	}

	component LabeledKeyEntry: KeyItemEntry {
		id: labeledKeyEntry
		property alias text: keyLabel.text
		Label {
			id: keyLabel
			anchors.centerIn: parent
			font.pixelSize: labeledKeyEntry.keySize.height / 3
			font.bold: true
			color: 'white'
			antialiasing: true
		}
	}

	KeyModel {
		id: keyModel

		KeyEntry {
			id: keyWithGrabbedVisibleItem
			image: grabber.image

			onKeyPressed: {
				itemToGrab.startAnimation()
			}
		}

		KeyItemEntry {
			id: keyWithGrabbedInternalItem
			keySize: deck.originalKeyImageSize

			property real itemScale: pressed ? 0.8 : 1.0

			Rectangle {
				id: internalItemToGrab
				anchors.fill: parent
				scale: keyWithGrabbedInternalItem.itemScale
				color: 'darkgreen'
				radius: 8
				antialiasing: true
				onScaleChanged: {
					keyWithGrabbedInternalItem.updateKey()
				}

				Label {
					id: intText
					anchors.centerIn: parent
					font.pixelSize: keyWithGrabbedInternalItem.keySize.height / 3
					font.bold: true
					text: 'Text'
					color: 'white'
					antialiasing: true
				}
			}
		}

		Instantiator {
			id: inst
			model: deck.keyCount

			delegate: KeyEntry {
				imageSource: pressed ? deck.pressedImage : deck.normalImage
				onKeyPressed: {
					console.warn('Instantiator delegate', index, 'pressed')
				}
			}
		}
	}

	PagedKeyModel {
		id: pagedModel

		sourceModel: keyModel
		keysPerPage: deck.keyCount

		prevPageKeyEntry: LabeledKeyEntry {
			keySize: deck.originalKeyImageSize
			text: '←'
		}
		nextPageKeyEntry:  LabeledKeyEntry {
			keySize: deck.originalKeyImageSize
			text: '→'
		}
	}

	StreamDeck {
		id: deck

		property url pressedImage: 'qrc:/examples/images/Pressed.png'
		property url normalImage: `data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAgAAAAIACAYAAAD0eNT6AAAACXBIWXMAAAsTAAALEwEAmpwYAAAgAElEQVR4nOzdeZxVdf0/8Nf73HvnzsoquESKCs5yWUQQEZcks0yzNNP2Ra30a7tL2d6vb4stLpWWW+W3skzTrJRcMsgFUQEVuDODopAiiogwzDBz1/P+/XG5MAyzzznnc5bX8/GYhzJz7zkvhpnzeZ/P+SwCIvI1XTe5Eh21E2DZ+8DWfSAYD8VYAONgyRgoRkFkNIBa2FoH0WpAqiBSDdUkgEoAiZ0fcQDWzo/u7J0fBQD5nR8ZiGSh2gloF1Q6YUk7gA6otkGwHbZuA/AGBFuh2AJLXodtvY7ajs1y8PqMN98hIhoOMR2AKKpUIXhqyj5IVB4EFN8M4EBA3gzB/lBMgmB/ABMAjDEcdbi2AdgMxSsQbIDiFUBfAvAiEHsJ+cx/MWvt6yJQ00GJoogFAJGLVGGhteFAFGQqxDoMwBRADwVwKICDANSYTWjcDgD/BfA8IM8DWAu1n0Vcn0ND64sisA3nIwotFgBEDtDbEMP0hkNRiE+DaApAE6CNEDkMqlWm8wWSSBdUnwWkBUAzVNKIF1ZjVevzcjaKpuMRBR0LAKIh0iWTqjCmbiZsHAHgcEAOh8g0NvQeKRUGqwF9GsDTsLAC29qfkfkbukxHIwoSFgBE/dBFJ8Qx8bXpAOaWPuRIAE1QjRmORt2JFAE0A/okgCcAPIHXJq6SBYsLhpMR+RYLAKJudNkho1GdPAZqzYfax+xs8KP+nD6odgD6JMR6FGIvQWf2UZnzQpvpUER+wQKAIk1bGsajIMfDkhOgOB7ADOw9RY7CwQawEoKHYOtixPUhaWzdYjoUkSksAChSdMmkKowe/RaongRgAYDDwd+DqFIATwNYBJEH0Nb2H44joCjhhY9CTRWClsYUinIyBCcDOBZA0nQu8qUsgEeguBcxvReNLWmuUUBhxgKAQkeXza5GMvtWWPapULwLwCTTmSiQNkBwN2zrHrRve5C9AxQ2LAAoFLSlYTyKcirEOh2q7wBQbToThUonRO6D2nchpvdw7ACFAQsACixtrT8AhfgZgJ4JkeM5NY88IVKE6kOA3IF44a/SsGaj6UhEw8ECgAJF06n9oDgTqmdDcBz4M0xmKRQPQ+Q2CO6QVPpV04GIBosXT/I9fW7KKOQqzgTkg1A9EZymR/5kQ+RBQP+EitwdMnXtdtOBiPrDAoB8SRedEMc+r58M2B+F4N0obWlLFBQZKP4OWL/H6/vcyxUJyY9YAJCvaLopBcW5AD4MYF/TeYgcsAnALRD8RlLNadNhiMpYAJBx2lpfh0L8A4CeB+Ao03mIXPQ4IL9GvHCrNKxpNx2Goo0FABmj6aZZUFwA4EMAak3nIfJQB4A/QnCdpJqfMh2GookFAHlK102uREfVWYBcCME803mIjFMsBfSXqO26XQ5enzEdh6KDBQB5Qp+ZMQnxwv8A8imoTjCdh8h3RDYDeiMK8V/JzJUbTMeh8GMBQK7S5sZ5sOUiiLyXC/UQDUJpoaE7YemV0tSy1HQcCi8WAOQ4vQ0xNKVOB+yLAJlvOg9RcOkSwLoSzem75GwUTaehcGEBQI7RJZOqUDfq4xBcBGCq6TxEIfIcFFeiffv/cVMicgoLABoxfW7KKGQTFwByETh3n8hNmwC9Esn8dVxpkEaKBQANW2kHvtgXAf0sgDGm8xBFyDZArkGseDV3JqThYgFAQ6YrpkxAReJiQD4Dzt8nMqkD0GuRy18hR6zdbDoMBQsLABo0XXPYPsjHLgHkswBqTOchol12AHoNEsWfSv2zr5sOQ8HAAoAGpMsOGY3KyosAXATe8RP5WQeAK5HJXClzXmgzHYb8jQUA9UmXza5GsvMzELkMwDjTeYho0N6A6uXIVl8rc5Z3mg5D/sQCgPaii06IY8KmTwDy/wAcYDoPEQ3bRkC/jc373swtiaknFgC0iyoEqxtPgchPADSazkNEjmmB6qWY1rJQBGo6DPkDCwACsGtnvisALDCdhYhcswiCi7kDIQEsACJP06n9AHwPqueCPw9EUaAQ+Q2Ab0gq/arpMGQOL/gRpesmV6Kz5otQ/To4sp8oijog8n1UZK+SqWuzpsOQ91gARJA2N54CW34O4FDTWYjIuOdh6eelqWWh6SDkLRYAEaLP1B+MWOxqAO82nYWIfOfvKBa/KDPXrDMdhLzBAiACNJ2qgK2XQPBNAJWm8xCRb2Wg+F9Y8lNJpXOmw5C7WACEnDY3Hg+1boBqveksRBQQImsg9qelqeUh01HIPSwAQkrTqXGw9acQnGM6CxEFlOK30NjFMmPVVtNRyHksAEKmtJhP01kQ/ALARNN5iCjwXoPic5jWfDsXEQoXFgAhos2N+6Mov4TgdNNZiChkFHchphdKU8srpqOQMyzTAWjkVCGaTn0CtjSz8SciVwhOhy3Nmk59QpU3j2HAf8SA09b6A5CP3QjBKaazEFFEiNyDWOHT0rBmo+koNHwsAAJKFYJ06gOAXgtgrOk8RBQ5WwH5DFLpWzk2IJhYAASQtjSMR9G6DsD7TGchosj7C0TOl1T6DdNBaGhYAASMrmo8CSI3AzjAdBYiop02QvUTMr3lAdNBaPBYAASErptciR1VPwDkS6azEBH1Tq9CTdfX5OD1GdNJaGAsAAJAV05vgFX4MyAzTGchIuqfroQdf7/MWNVqOgn1j9MAfUwVoquazoVVXM7Gn4iCQWbAKi7XVU3ncrqgv/Efx6e0tb4Ohdh1AD5kOgsR0TD9EfHiBdKwpt10ENobCwAf0lXTZkDsvwCYajoLEdEIPQu1zpLpq1eaDkJ74iMAn9F00zkQ+3Gw8SeicDgMYj+u6SZuTOYz7AHwCV0yqQqjR/8CqueZzkJE5AqRX6Ot7XMyf0OX6SjEAsAXtKVhMorWnQBmmc5CROSypxCz3yuNretNB4k6PgIwTNNNJ6JoLQMbfyKKhlkoWss03XSi6SBRxwLAkJ1T/C6B4n4A403nISLy0Hgo7tdVTZdwqqA5/MYboEsmVWHUqJvAKX5ERH/E9u2f5LgA77EA8Jg+fdibkEj8DaqzTWchIvIHXYZC8XQ5/NmXTSeJEhYAHtKVDbNhWX8HN/IhIuppI2z73TKjdbnpIFHBMQAe0XTqfYjFHgYbfyKi3hyAWOxhTae4zblHWAC4TBWi6dRlUL0dqlWm8xAR+ZZqFVRv13TqMg4OdB+/wS7SRSfEMWHzrwD9pOksRETBojdi874XyoLFBdNJwooFgEt2bubzZwDvNJ2FiCig/ol48f3cTMgdLABcoOnUflBdCC7uQ0Q0Uk9B5BRJpV81HSRsOAbAYfpMfT1UHwMbfyIiJ8yC6mP6TH296SBhwx4AB2k6NXfnnT9X9iMictaWnT0BT5gOEhbsAXCIrmo8CaqLwMafiMgN46G6SJsb32Y6SFiwAHCArmg8DyL3AKg2nYWIKMSqYctCTTedZTpIGLAAGCF9oumrKMiNABKmsxARRUACilt1VdO5poMEHQuAEdBljZejCz9AFcdSEBF5yILg17qq8QumgwQZC4Bh0mWpa7FdvoJacCglEZEJIldruumrpmMEFQuAIVKF6JNNv0GbXogEgKTpREREEab4ga5q+g6XDh46fsOGQBWCZY23YLt8EApgHwBx06mIiAiQHyOVvkwEajpJULAHYJBUIXgy9addjX8l2PgTEfmGfhnp1OXsCRg8FgCDULrzT/0R7fr+XbVlrdFIRES0FxYBQ8ECYAA77/z/iO36gV2NfxV4909E5EssAgaLBcBAljX9Gu3dGn8B7/6JiHxNv4zVTd82ncLvWAD0Q5elfontOGePISVVAGKmEhER0aAIvs0pgv1jAdAHfTL1Y7Tp/+zR+PPun4goOBQ/4GJBfWMB0Atd3vQ1tOule00mqQK/Y0REQSJyNZcN7h0HSfSgTzVeiG1yLYo9viAAJoAFABFR8NgQfEBSzbebDuInLAC60WVNZ6EDf0ahl+9LNYBR3mciIiJH5KF6qkxvecB0EL9gAbCTLq9/KzpiDyDfxz3+BHDwHxFRsHVCZIGk0k+YDuIHLAAA6LKG6ei0liPXx5a+lQDGeJuJiIhcsQXF4jEyc80a00FMi3wBoCvqD8CO2LPIoqbPF3HNfyKiMFkPkaMllX7VdBCTIj2kTVvr69AZf7rfxj8JNv5EROEyGaoLtbW+znQQkyJbAOiiE+J4I/4EMjqh3xf2XRoQEVFwzUIh9mdddEJkb/EiWQCoQlC9+V50akO/L0wAqPAmExERee6dmLDpl1HdNyCSBQCWp65Bu5444Ot4909EFHLyKTSnvmI6hQmRq3p0WeOF2C7Xwh7ghTGUBv9F7jtERBRBImdJKv0X0zG8FKnmTVc0Ho92WdTnXP/uRqG0+A8REYWfSBeKxeNkRuty01G8EpkCQJcdciA6K9cgh8oBX2yhtPBPZL47REQEYCMKhbly+LMvmw7ihUiMAdAlk6qQq3xiUI0/UFr4h40/EVHUHIBE4m+6ZFKV6SBeCH0BoAqBNerf6MS+g34TB/8REUWT6myMGnVTFGYGhL4AwJNNV2MH5g369UlwzX8iomj7EJqbLjIdwm2hrnB0RepMbNO/DDjiv7uxKBUBREQUZTYEb5dU84Omg7gltAWAplNTsEWb+9zgpzec+kdERLttQcyeI42t600HcUMoHwHokklV2IolQ2r8AaAKbPyJiKhsPIrWnWEdFBjKAgDx0QsHXOO/N6H8JyYiohGYhdGjf2E6hBtCVwDok01fQ7ueMOQ3VoKD/4iIaG+q52m66RzTMZwWqg5vXTbjKHQUHkNhGH8vDv4jIqK+ZWDbc2VG6yrTQZwSmgJAW+vrsDn2MrIY+v7OMZRW/iMiIurbs4gX50jDmnbTQZwQikcAqhBsi90/rMYf4LN/IiIajMNQiF0XlkWCQlEAYHnj14e02E9PLACIiGhwPoTV4RgPEPgqRlekZqLdXoG8DK+YqQAwztlMREQUap2wY7NlxqpW00FGItA9ALpuciW69MFhN/4ABrk9EBERUVk1rOKtum5yoFuQQBcAeK36VnRh/LDfL2ABQEREwzETO6p+YDrESAT2EYAubzobbfjzkNb576kSwBinEhERUeSovl2mtzxgOsZwBLIA0JaG8dhsbUBuhPfvY8AeACIiGomNEJkuqfQbpoMMVeAeAZSm/MkDI2782f1PREQjdwBUrw/i1MDAFQBYkfoCOmXWiI/DVf+IiMgZ70M69QHTIYYqUBWLPjNlErZVrEMe8REfjEv/EhGRc7YiXpwmDWs2mg4yWIHpAVCFYEfFA440/oLS/H8iIiJnjEUxfkOQHgUEpgDAk02XoQsNjhwriYD1fRARke+pnorm1MdNxxisQDSD+vRhb0JbYj3yOvK7f4Dd/0RE5JZtsLRJmlpeMR1kIL7vAVCFoCt2r2ONP7v/iYjIPWNQlF8G4VGA7wsArGi8EJ0yzbHjsfufiIjcJDgdq5vOMh1jIL5uCjWdGoc39BVkHbxnHw3u/kdERG57DXasQWas2mo6SF/83QPQrnc52vgDXPyHiIi8MBFSvMJ0iP74tgDQZxpOQyeOc/SgFfB5nwcREYWG4BxtbjzedIy++LI51HSqAtvwGrp0tKMHrgNQ4+gRiYiI+iayBsAMSaVzpqP05M8egB32NY43/gCn/hERkbdU6wG92HSM3viuB0DTqSnYrM+i4HC2GIAJjh6RiIhoMDIoFptk5pp1poN0578egHa9y/HGH+DgPyIiMqUSsdhVpkP05KsCQJc3fQQ7kHLl4Fz8h4iIzHmPNjeeYjpEd755BKDrJldiQ/VmZFHr+MEFwET46G9LREQR9DySuZRMXZs1HQTwUw/A69VXudL4A0ACbPyJiMi0Q5FLfsl0iDJfNIu6ov4AbI+9hIJLBUntzg8iIiKzOqDWFJm+epPpIP7oAcjH/uRa4w9w+h8REflFLSz9vukQgA96APSp1DHYqo/AdukEfP5PRET+ohDMllTzUyZDGO0BKG31a//BtcYf4O5/RETkNwLFFaa3DDb7CGBZ40fRJZNdPQen/xERkf8swGqz0wKNVR+66IQ4Eps3I6NjXD3ReJRmARAREflLCzZPnCELFhdMnNxcD0Ddq19D1uXGXwDEXT0DERHRcDViwqZPmDq5kR4AXTa7Gp1dW5BzeYHeJICxrp6BiIhoJDZi+/YpMn9Dl9cnNtMDYHf+2PXGH2DXPxER+d0BqKv7rIkTe94DoMsOGY0dla8j70Hn/DhwECAREfndG8hkDpE5L7R5eVLvewDsyms9afwBNv5ERBQE41BZeZHXJ/W0B0DXHLYPXo29irzEXD9ZAqUZAERERP7XgUThYKl/9nWvTuhtD0Bb/DpPGn+Az/+JiChIapGPXeLlCT3rAdAVUyagveJV5D0qOsYAHgwzJCIicsoO5HIHyxFrN3txMu96AAoVv/Ks8QfYA0BEREFTg4rExV6dzJMeAG1pGI/X5DXkxZsCQADs68mZiIiInNSBmD1ZGlu3uH0ibxrk7bFfeNb4A7z7JyKioKpFUb7gxYlc7wHQ56aMwsaKLZ5N/QOAGgB1np2NiIjISduQzB0kU9dud/Mk7t+Vb0v8yNPGH2APABERBdkYZBMXuH0SV3sAdMmkKhRGbUPO4yV59gE3ASIioiDbhO3bD3ZzjwB3ewBio77heePPHQCJiCj49kXdqI+7eQLXCgC9DTHk8Dm3jt8nNv5ERBQGgov0Nri2eJ57PQAHN30SWQND8bxZZ5CIiMhtU9GUOt2tg7tXAOT1W64duz8cAEhERKGhX3LryK4MAtQVTSdiK/4FdePoAxgLIGngvP2wbeC/r9hY97KNLW028gXTicgkywLG1ArevJ+Fww6KIcHHVo4rFIG1Lxbx4qs2tm5XFG3TicikeAwYN1pw0P4WDpkUQ8z7fXBHRjBPUs2PO31Ydy49OfzUSOMP+GYMQL4A3P9YDn9bnMcjTxXQ1mHqG0J+VpEAjkzF8a7jK3D6ggRG1Xi6QWeodHQq7n44j7sfymPpygK6svydo73VVgvmz4zjtOMTOPW4CiSDsG284mIAZzt9WMevNrosdSDa9b8oOn3kQfDBEsCFIvD7u7O45tYMXt3CCxANXnWl4GPvqsDnPliJMXUsBAaro1Nx3V+yuOmvWbTv4O8cDd740YILzqrEeacn/V0IiBRRiE2WmSs3OHpYJw8GAPpk021ow1lOH3dQ4iitAWDIqueK+OJPO9G6zkT1Q2ExbrTgh5+rxruO54CWgfz7iTwuvaqTxTaNyOQDLFx1STXmTvNJF3JvRL4vqfQ3HD2kkwfTdZMrsaG6DVmP5/6XVaK0DbABt92fw5ev7uTzfXLMuacn8Z0LqoL3vNIDqsAVv8/gqj9kTEehkLAs4FufrsKn3uuzQWRlIptRkX2zTF2bdeqQzl5attR8wVjjDxibAnj9X7L40k/Z+JOzfnNXFhf+YAcKRd7ddmfbwGU/72LjT46ybeA713XhR7/NQP34K6c6AZmEo+MAnC0AMvpFR483VAZ6b267P4fv3uDaSo0UcXc/lMdlP+vy5wXJkMt/m8Ef7nHsJohoDz//UwY33OHXny+50MmjOVYA6IrpRyOH/Zw63rB43AOw6rkivnx1p7cnpcj507053LLQrxckb93zcB7X/pl3/uSu793UhcdW+rBLVzBP002znDqccz0AucLlMD3X1sMCoFAEvshuf/LId67L4KVXTf+CmbWlTVlwkydsG/jiTzrRmfFh15vCsV0CHSkAtLW+Dlkc68Sxhk3gaQHwu39kOdqfPNOVVfzg19F+1HTF77qwrd2HF2QKpQ2bbPzqdl/2vH1IW+sdWWbfmR6AzvglyIvZscoeNv75AnANuyHJY3//Tx5rX4pm0blpi+KWhTnTMShibrzTl2tL1KIQ/4ATB3Km0e7S8x05zkh4WADctySPTZx3TAb87h/RbAT/+M8sCtGsfcig9h2KuxbnTcfYm+q5ThxmxAWAPtN4BDKm19+DpwXA3//jwx8IioS7H87BjuBQAP7OkSl/W+TDors0GDA10sOMvAegS/7X2Lr/3Xn0AMK2gYeW+/AHgiJh0xbFmv9G61b41S2KZyP2dyb/eDJd9OtgwBH3Aoyo2dRFJ8SRx9tGGsIRHvUArN9oo50DkcmgZ56NVmP4dCun2pA5haKiZZ0vu90+rItOGNHqNyO7bx615SPIGVz5rzuPegDWbfTlDwJFyLqXo/UzyN85Ms2nv3P7Yp9N7xjJAUbWbObsL/mi+x/wrAfg9a2+/EGgCInaz+CWbdH6+5L/+PZn0LI+OqK3D/eN+tyUUcjq9JGc3FEeFQA59kaSYVFbfCpqf1/yn6xfx6CqvkefmzJquG8ffg/A1uQXUXB+O+FhEbiwsXHvKv3xwIMizNf7lrsgWeGPywxFV5VPNwgEUIlcxZnDffPwC4CCnjPs9zrNwyWIJo7jxYjMmjguWvsDTxjL3zkya58xfv4ZlA8O953DupJoc+P+yGLycE/qOA+vh4dOMrTnMNFOh06KVgEw5c3R+vuS/0x5s4+v+6on6qppw1qLZ3i/WTusS+CnmUgeXh/eNNHChLG8IJE5RzQa2PfaoMPro/X3JX+pSgoaDvZxAQBYkOKwHgMMryXL4f3Dep9bPGyPRYAFR/KCRGYc/CYLB+0frQJ07CjB4fW+vgBTiB07K46E3y/5Yp09nLcN+Uqiz0yZhJy+aTgnc43H18PTFyS8PSHRTqcvqID4+XGkS854a8RGPpJvBOJ6r3q8NjfuP9S3Db3pzCQv8lX3P+B5AXDcrIS/nwlRKMVjwEdO9e9wZDed/fYK1FZHsPIho/YdLzj1uEAUnwIbZwz1TUNvOvN435Df4zaPrwuWBVz8sUpvT0qR97HTkthvfDQbwVE1gvPPjGbxQ+Z84UNV/u/+30WGPA5gSAWAtjSMR17fPNSTuM7AI9HTjk9g/szA/GRQwE0Ya+Hij0a76Pyfs5M4aH/2vJE3pk2J4cOnBKD7v0zkLdrSMH4obxla09mOz/iu+x/wvAcAKA0GvOqSaoypi+YdGXnryourIv+zVpUU/PwrVYjHov19IPdVJQW/+Ep1sH7WVGMoyqlDecvQCoBC7ENDer1XDA2KnrSvheu/UYM4b0rIRV89txJvnRugOxEXzWmK4wefi3ZPCLlLBPj5V6pw2EEBvLCLdfpQXj7oplOXza5G3p469EThduysOK7/Rk2AnhNRkHz+g0l85v1s8Lr78ClJfPv8KtMxKIQsq9Sze8qxgRj4tzfVd+iy2dWDffng75216wPIiz8nIBtOdfIxCdx6eS3Gjw5QdxH5WjwG/PDz1fjKOVWRnPY3kE+fmcQ1l1Vzbw5yTF01cPN3a3HWSYH+oapGMvvWwb548E1nEf5Z+78nH1wg582I41/X1+HkY9hVSyPTdEgMd/+8Dh97V6AvRK47460VuPeXdZjdGMCuWvKV44+I41/Xj8KJc0PQlWvpKYN96aCaTlUIHm7sRE782Re5L3xRBJQ9+nQBV/4hg6UruY8pDd4hkyx89v2VOPNtiWANPjLMtoG7H87jZ3/MoHWdH0cpk18dXh/Dlz5SiRPnJsLU07YBqeYDRaADvXBwBcCqaXPwmv3kwIczZD/TAXq3Zn0Rf1ucw0MrCkg/X0TOr3tKkxEiwNQDY5g/M45Tj0tg3vQ4LH8+ZAsEVWB5SwF3P5THI08V8Ox/iyjaplORn8RjpR62445I4LTjE5g2JRamhn83S6dLU8vqgV42uAJgaeOv0SHnjjyVCwSlHgCfKxSBl18rYut2IJf3ayVFXojFBKNrSxtLVSXDePXxh2wOePk1G9vaFYUif+eiLBEXjB0leNNEKxoDtgWXSKr5ioFfNgj6aNOL6IL/FgACAlMAEBEReeRBmdb8toFeNGCHoy6ZVIU8JjmTyQW8gSIiIuruWF0yacC5sgM/cayoPRsFNrNEREQBkcTo0W8Z6EUDFwB5+bAjcdzC0oSIiGhPqicN9JKBC4CCNceRMEREROQNxQkDvaTfAkDXHLYP8jrWsUBuYA8AERHRngSzBtodsP8egK2xD8Pv82g5u4eIiKgnQUGO7+8F/RcAap3haBwiIiLyhmX1OxCw/wIgrzMdDUNERETeUB1eAaDLDhmNAsY4n8hhHANARETUmxm67JDRfX2x7x4AqTjb98//AY4BICIi6p2Fqsr5fX+xL7nYu12JQ0RERB6RY/r6St8FQF45/5+IiCjI1B5aAaCLTohDdaJ7iRzERwBERER9kCN10Qm97oHYew/A6E3zkRfuTE5ERBRsNZi4eVpvX+i9kc/Jma7GcRJ7AIiIiPqhR/X22d4LABvHuprFSSwAiIiI+jO3t0/2XgAUdKqrUZzGIoCIiKh32vug/r0KAF0yqQoFqXM/kYNYABAREfVOrJQumVTV89N79wDEak8MxAJA3bEAICIi6p1qDKNHzej56b0LAFtO8SSQk1gAEBER9U11ds9P9VIA4EhPwjgpaD0WREREnpK9NvfbuwAoyqGeZHESewCIiIj6M6vnJ/YoAPQ2xFBU/+8A2BMLACIiov6k9DbEun9izx6AqanpKErwNtjlIwAiIqL+VGN6wx49/HsWAFmc7Gkcp7AHgIiIqH+F+B5LAu9ZANg6z9MwTimaDkBERORzoqnuf+xRAEiDp2GcwkcAREREA2nq/oeePQAHeBrFKXwEQERENADd4yZ/VwGgCgtF1HofyAHsASAiIuqfSL3q7nZ/dw/Aimn1KCJ4MwAAjgEgIiIaiGoVWhsOLP9xdwGQs99iJJAT2ANAREQ0sILs2u13dwEgAVwCuDsWAURERP0T67Dy/+4uAIoBnQFQxgKAiIhoIFPK/9OtANA3G4niFI4DICIiGoDuWg1wdwGgMt5IFqewB4CIiGggexYAqhDYWmUujwPYA0BERDSQg1RLM/5KPQBP1e8PO6BTAMtYABAREaYaDBgAACAASURBVA2kBk9N2QcoFwD5xFGBX02PBQAREdHAEpUHAeUCQHVavy8OAhYAREREg1B8M1AuAGwN9hRAgAUAERHR4BwI7CoAcJDRKE5hEUBERNQ/SyYBux4BYKLRME5hAUBERNQ/G/sDu3sAxhkN45SC6QBEREQ+t0cPALTGZBbHsAAgIiLqn2q3HoAiKoyGcQofARAREQ1kIgBYum5yJWyxBnp1ILAHgIiIaCBjdN3kSguvj5oy8GsDgj0AREREA+uonWChWDh04FcGCHsBiIiI+mfZ+1iw9WDTORzFAoCIiKh/ouMtaGk6QGiwACAiIuqfjX0siEwwncNRLACIiIj6pzLGgq0sAIiIiKJlnAXRMaZTOKoABH5rYyIiIjdZMsaCSp3pHI5jLwAREVHfVEdbsLXWdA7H5U0HICIi8jGR0RYglaZzOI49AERERH1Tu8aCImk6h+PYA0BERNQ3seosKBKmcziOPQBERER9U622oBo3ncNxChYBREREfau2ACtmOoUrcqYDEBER+ZRItQVFOLYC7onjAIiIiHqnmrQAFdM5XMECgIiIqC9JC4pwFgBcEZCIiKgvCQsIaQEAcBwAERFR7yrC+fy/jAUAERFRb+JWqLvJOQ6AiIioN5YV4gcApQIgzAUOERHR8Fjh7gFQsBeAiIioF+EeAwBwHAAREVEvwv0IAGABQERE1ItwPwIAOA6AiIhob3b4ewA4DoCIiKgnO/w9AACQNR2AiIjIVwpWBB4CcBwAERHRnnIWoOEvAAoAbNMhiIiIfCNvIQpD5BTsBSAiItota0EkGvfGHAdARERUIpK1EJXOcRYAREREJaqdFgQF0zk8YQMR+ZsSERENpNNClGbJZ0wHICIi8gGRTgsSoc5xFgBERESA2u0WRKJTABQAFE2HICIiMkysHRagO0zn8BSnAxIRUdSptlmwpMN0Dk91mQ5ARERkmEibBdjbTOfwVA5RWPqIiIiob7Zus2DhddM5PMfBgEREFG1vWFDdbDqF51gAEBFRlIluswB52XQOz+UQlfUPiYiI9mbhdQvAi6ZzeE7BpYGJiCi6VLZYQPF50zmM4GMAIiKKKtt63cJ4aTWdwwjOBiAioqiq7dhsScOadkgEm0IFewGIiCiKtsnB6zMWACAWoQ2BuuOiQEREFD2vAUCpABB0Go1iSh7cG4CIiKJF5BWgXAAA0VoNsEzBXgAiIooWWzcA5QLAKnUHRBLHARARUZRY6NYDIKVqIJIKQERHQBARURTt0QMgeM5oGNP4GICIiKLjRaBcAMRktdEopnWBawIQEVFExF4CdhUAxSeNZjGNawIQEVFU5DP/BQABAFUIFjUVYZf+HEkVAMaZDkFEROSqHUg114lALQAQgUKQM53KqBy4JgAREYXdf2Xn6r/Wrk9Z2Gosjl9EczkkIiKKjl0bAO4uAOLYaCSKn3AwIBERhZr0UgAo1hrJ4ic2OBiQiIjCbFdbv7sAEF1hJIrf8DEAERGFldrPlv93dwFQYT9iJIzf5MGVAYmIKJziumvhv13T/lQRw7+bCnwGDqAKwGjTIYiIiBwk0oWmdK0IbKBbD4AIiogrF8UFSoMBOSWQiIjCRHVNufEHuj8CAABLorsrYE8shYiIKFSktfufrB5f5UyAMg4GJCKicGnu/oeePQDLPY3iZzZYBBARUXiopLv/cc8CIGb/y9MwftcJLgxEREThEC/ssfPvngXAMy2LI7wd0N4KALKmQxAREY1YJ1a1Pt/9E3sUAHI+8ohhh7eZfI6PAYiIKPjScvae89t6DgIEYnjJszhBkNv5QUREFFxP9fzE3gWApU97EiVI2CdCRESBps/0/EwvPQDygCdZgiQLLg9MRETBJXvP8tu7AMh3/NWTMEHDXgAiIgoikSLatq/s+em9CgA57sWtiCs3xe0pg9KsACIioiBROy3zN+y1vu3ePQAAEJMXXQ8URB2mAxAREQ2RyLLePt17AWDhSVfDBBV7AYiIKHie6O2TfRQA1j9cjRJk7AUgIqJAkcd7+2zvBcDWzF2wuAhurzLgjAAiIgqKHXhtwurevtBrASCnrM0ihjZ3MwUYewGIiCgQ9ElZsLjXh9e99wAAQEx6rRgIpXUBuDogERH5nViP9vWlvgsAC/e7EiYs2AtARES+p8MoALLZ37qSJSxy4E6BRETkZza6Mkv6+mK/m//q4lQnClrlfKaQiAMYjwG+ixRkmRyw5OkCHluZR+v6Il5+TbG9Q6HKMbJRVpkU7LePhSlvjmF2YwwnzElg4jheCMh3npZpzbP6+mK837fGsAYFHO54pLAooDQrgCVS6Dz3YhE33JHF3xbnsaOLjT31pFi/0cbSlQX84R5ABDjm8DjOOz2Jk+YlIKwFyA9E/tPfl/svACz7PkBYAPSnHSwAQmTrdsX3b+rCrfflwJt8GixV4JGnCnjkqQIOr4/jB5+rxMzD+r+8ErnOtvstAPoeAwAAYt3E7u0B2OCAwJBY8kwBCz7Vjj/dy8afhu/pNQW86/MduPqWDGzbdBqKMEVcH+rvBQM27/qfph3Io9q5TCEkACZgoHKKfOzW+3L48lWdKPKCTQ561/EJ/OIrNahImE5CkaNYIdObZ/f3koGbrBjSjgUKK0XpUQAF0q335XDxFWz8yXl3P5THBd/fgUKRXUrkMcHigV4ycAFg4a9OZAk9LhEcSI+tLODLV3WajkEhdt+SPL53I3dYJ4+JPDDQSwYuAIod1zkSJuzYCxA429oVF/6Ad/7kvhvvzOLBJ7iVKHkmi7a2fgcAAoMoAOS4F7eiAm84kynkcgC6TIegwfrhbzJ47Q22/uSNy37Wia4sHwWQJx6R+RsGbI0GN2zNkj6XEqQeOgDuo+h/L2yw8cd/cilH8s7GzTZu/js3ESEPCP45mJcNrgCI4aYRhYmSIvgoIABuuINTtMh7N96Z4YBAcp/ofYN52eAKgHnpfyAmfIA1WJ0orRJIvpTNAXct5ohN8t6mLYqHlvPiQK7agMaWQc3eG1QBIAJFXNeMLFPEbDcdgPry+OoC2nfwLozMuP8xFp/kIpF/iAzuQfTgl66xcPuwA0URBwT61uOreAdG5jy+umg6AoWZLQsH+9LBFwC5zp9xWeAhakdpqWDylZZ1vACTOc+/VESeNSi5oxPt2x4c7IsHXQDIgvXbkMDG4WWKKBt8FOBDL7/GqozMKdrg9FNyh8h9g5n+Vza01etj9t1DDhR1GZQeB5BvdHTy+T+Z1c7FJ8kNat81lJcPrQCQ2E+G9HoqaQPXBvAR7tVOxnG7SXKaSBExvWcobxlSASDz02uRwNahpSKuDeAvo2u5bSOZNWYUfwbJYar/kcbWLUN5y9B/CmNy/5DfQ6UZAZz94wtvmsguADInEQcmjOXPIDlN7xjqO4ZeAFTgcs4GGAZF6VEAGTdtSsx0BIqwhoNjiLEDgJylw9m5d8g/hjI3/TTiwqZsOArgowAfOHpG3HQEijD+/JHjRB6SppZXhvq24dWhMdw7rPdRaZlgzgowalZDDBPG8haMzHjnMQnTEShs1L5tOG8b5lVQ/3d476NdjwI4CNiYeEzw/ndUmI5BEXToJAtHptgDQI6yobEhP/8HhlkAyLHNaSQwpNGG1E0RXCDIsPNOTyLJGoA8duH7KzkNlZwl8qBMX71pOG8dfj9oXIc84IC66UJpkSAyYuI4wQXvS5qOQRHSMNnC+97G7n9ymv5puO8cfgFQZX972O+lku3gXgEGff5DVZh6IGcEkPtiFnDFxdWIx3j7T47KoCI3rO5/YAQFgByxZiMq5KXhvp9Qavw5n8KYygrguq9Xo6aKF2Vy1zc/XYXD6/nsnxwm8jeZunbYD5RHNhQ6pr8e0fsJyALYYTpEdDUcHMNN365BBXtmySWfem8SnzyDj5vIBbb9+5G8fWQFQFvucljsxB6xDnCVQIOOPyKO33+vFnU17AkgZ332A5X49vlVHPhHbtiE1/e9byQHGFEBIKeszaJCVozkGITSlMBt4NRAg46dFcc9v6hD0yEcE0AjV1cjuPar1fjquRz1T665RRYsLozkACNfDSWG7474GFSaGsjxAEYdOsnCwmvq8LXzKtkbQMN2+oIKLL5pFE5fwHmm5CLBb0Z+CAfo4qZ2FFDrxLEibxSAatMhqK1DccvCHG67P4fnXiyajkM+N6ZOcNrxCZx7ehKHHcReJHKZYqlMbz56pIdxpgBY0ngzOuXjThwr8gTAOAAclOYb6162sXRVAa3rinj5NRvbdyhsjnyJtOpKYOI4C1MPjOGIxhhmNcQ4xY88JJ+WaekbR3wUJ6Loqmn7YrP9CmzuE+iIGIB94NC/DhERhUgHRPaXVLpjpAdyZEcUmb56ExJoceJYhNJ4gG2mQxARkQ/90YnGH3CoAAAAxC0OBnRSFqXpgURERGWC65w7lIP0P03bkUedk8eMNAEwFgAHExMRkUOD/8qc3RQ9gVscPV7UldcH4CB0IiKC/tLJozlbAFRkLoOlHB/tJBvAVnCRICKiKBPZjMr8bU4e0tECQOa80IYK6zEnj0kACuAiQURE0XaDTF2bdfKAzvYAAEC8+AXHj0lABhwUSEQURSJFFGKODf4rc7wAkHmty1GBF50+LqFUAGRMhyAiIk+p3ikzV25w+rDO9wAAQAzfceW4VBoUyJ0DiYiiQ3CFO4d1iT7U1IYcRrl1/EizAIxHacVAIiIKs0dlWvOxbhzYnR4AAIjL9a4dO+pslHoCON+CiCjk5Cq3juxeAbAq/XXEkXPt+FGXB5cLJiIKt+fQnL7LrYO7VgDI+cgjoY7OWaQecuD0QCKisFJcKWe7txScez0AANCZ/SwsdlS7qgucHkhEFD6b0L79/9w8gasFgJz0Qhsq9F43z0EoFQCdpkMQEZFz9EqZv6HLzTO42wMAANX2pyBcyNZ121HaQZCIiIJuG5J5xxf+6cn1AkCOWLMRSTzi9nkIpUGBHHZJRBRw+guZuna722dxvwcAACw5l70AHijvHsiFgoiIgqoDMf2ZFyfypACQ+em1SOrjXpwr8sq7BxZMByEioqHTa6WxdYsXZ/KmBwAAEvpx9gJ4xAbwBuDe5BEiInLBDuTyriz72xvPCgA5qvVZJGWpV+eLvHIRwEmYREQBodfIEWs3e3U273oAAKAKH4PFXgDPFMEigIgoGDqQKP7UyxN6WgDI7PRaJPQhL88ZeQWUxgSwCCAi8rMrpf7Z1708obc9AABQyH+EvQAey4M9AURE/vUGkjnPnv2XeV4AyIK1G5DUhV6fN/IKYBFARORHqpd7Me+/J+97AABArI8hphyj7jUWAUREfrMR7e3XmDixkQJA5qffQIX83sS5I49FABGRj+i33V7zvy9megAA4GU5HzGuXm9EuQhgHwwRkUkt2LzvzaZObqwAkLPTOVTih6bOH3ksAoiIzFK9VBYsNrZuq5g6MQCoQvBw0xbkMNZkjkiLARgLIG46CBFRpCxCqvlEMbhCrrlHAABEoIjphWbLkIgrLxbEvQOIiLyiEFxssvEHDPcAlOnDqVZktd50jkgTlHoCKkwHISIKMRuA4naZ2Xy26ShGewB2iVlncXEgwxSlngAOyyQicocNoF3yyOsXTEcBfFIAyPxVq1CBe03nIJSWDTYyIYWIKMTKj1sr9Ncyp+UV03EAnxQAAIB47gOIIW86BgFoA7DDdAgiopAoANgCII4OjMl90XScMt8UADJv7XZUyndN56Cd2gF4vjAlEVHI5FFq/BVAtV4oU9f65kGrLwYBdqcPNW1EDvubzkE7JQGMgQ9/UoiIfC4DYNvO/69GWuY3TzMZpyff9ADskiiexcbGR7LggkFEREPVidLjVACIQ1GlZ5iM0xvfFQBy9JpHkZD7TeegbspdWFwrgIhoYNt3fpTntlXLr2VWy3MGE/XKdwUAAGB07L2II2c6BnVjo1QE+ObpFRGRD21D6e6/rFLaUIPPmIrTH18WADJz5Q5UykWmc1APitIPN2cIEBHtqXyTlOn2OQFQXfiYpNK+vKH19dN2fbhxDbJymOkc1ItKAKPh858gIiIPFFBaQ6XnWKkaWSJHp48xkGhQfNkDsEs8+Q4Id673pQxKgwP5r0NEUZZB6c6/Z+OfQB6jcJqBRIPm6wJAjn56Par0Z6ZzUB/yAF4HOFqDiCKpA6XHoj0XshcAVXqppNJveB9q8HzfgasKwSONG5CVA0xnoX6MAlBtOgQRkQdslEb5Z/r4ejWacXTzNNO7/Q3E1z0AwM4tgyusU+Dzb2TkbUfvlTARUZiUl/Xtq/GPSxF11sl+b/yBABQAACBHpZ9Btd5kOgcNoPwsjOsFEFEYdaH35/1lAqDW/o5MX/2Sd6GGz/ePAMpUIXio6VXkMdF0FhqAoPRIoMp0ECIiByhK+6N0DvC6al2Do1sag3D3DwSkBwDY+SigSt4BKxjf2EhTlJbA7L4SFhFREJW38R2o8Y9LEaOttwel8QcCVAAAgMxNP40krjGdgwapE6VfHD4SIKIgyqA002mgjeoFQJ39VUmlX3Q/lHMC8wigrDQroGk9sjjQdBYaJD4SIKIgUZR6MLsG+foaXYl5LYcH6e4fCFgPAFCeFRA/AZZyf7qgKD8S4CwBIvK78ij/wTb+FchijL41aI0/EMACAADkqJXrkMTXTeegISp3p3HhICLyG0Vpn5PXMfjHlgKgWs6VxtYtruVyUeAeAXSnj6SeREbnmM5BQyQAanZ+BPonkIhCoYhSL+VQb05qdaHMaznVhUSeCGQPwC7x7ImIy2A7asgvFKUlNLlmABGZVp7bP9TGP4mt2LfrTBcSeSbQBYDMW7sdFXgv7yIDqvysrcN0ECKKHBulHfzaMPRNzWKqqI6dJAev72s9wEAIdAEAADI/fS+q8BvTOWiY2BtARF7rQulZf3YY7xUAVdb3Zfaq5c6G8l4o7p1LUwNTzyOrB5vOQiNQHhtQazoIEYVSEaXpfcNp+MuqsQJHN88J4qj/ngLfAwDsnBo4Ro5GnOPLA617b8BAC28QEQ3FSO76yyrQgXHFE8LQ+AMhKQAAQKav3oS49cFw9GlEXB6lIqDddBAiCrwCSiuStmFk65DEoKiTk6VhTWiuTKEpAABAjl19Jyq5a2Bo7ACwGSOr2Ikomrr3KI60b1gA1Mn3ZFb60RHn8pFQ3i/rI6kWZLTBdA5yUCWAOgAx00GIyPdyKD3rd2pgcY0ukaNbjnHoaL4Rqh6AXfI7jkZiwL2bKEjKqwjuMB2EiHzLRmnJcSc3IUtiK7T9bQ4dzVdCWQDIgvXbUGWfhJAM1KCdyntyczlhIuqpE6Vrg5Mz8+NSRLV1jMzfEMoF50JZAACAzG1dghp82XQOckF5UM82lKb1EFF0ZVFq+Ldj6Av69McCUCefkNmrWxw8qq+EcgxAd/pI6u/I6Gmmc5CLasF9BYiipohSj6Aba/EJgFFygxyZPt+Fo/tG6C+ZXCQoIiyUBglWmQ5CRK4qPwrsgnvbi9fgKcxrnh2W+f59Ce0jgDIRKAo7jkCMw8dCzUZpni/HBxCFV3lqcCfca/wrsQW6/ZiwN/5ABAoAYOegwBo5FpbyiXHYlccHbAVXEyQKiwxKDX87nH3O31MCOSSKR4Z10F9PkSgAAEDmpp9Glf3x8D/0IAClgUHl1b9Y9hEFUxalhXy8GPAbg6LGPk2OWrPO5TP5RmQKAACQo9fcgir5iekc5BHF7vW/nR4hTETuycHbnrzSRmSXypzW+z04m29E8n5YlzQtRCfeaToHeUxQGiRYi4iVvkQBUUCpm9/r5b9H4WaZ23yOx2c1LpoFgELwSFMzsuBywVEkAKpRmjrIQoDIvDxK6/ab2PejRpdgXsuxURj011MkL38iULTnDkdCNpvOQgYodo8m5qMBInPyKHXzb4GZxr9K1yOz71ui2PgDEe0BKNNV0/bFFn0BBa02nYUMKj8aqAE3GyLyQg6lItzkTp8V2IbqzGSZ80KbwRRGRboAAAB9dMY0ZAtPwUbcdBbygUqUCoGE6SBEIaMoNfg7YH6KbgIZ1Ei9zEm/aDiJUZEvAABAlzaehE65F3Y0H4lQL5IoFQIVpoMQBVx5Nk4nnNuhbyTiUkQ1jpa56SdNRzGNBcBO+njjx7BDbobN7wl1k0CpEEiCvy1EQ2Gj1Oh3wj/jbGJQ1Oq75MiWhaaj+AEvad3okqZL0IWfRHM4CPUrhtI4gWpEdOgs0SDlsfv5vp+upRaA2uK5MnfNb01H8QsWAD3okqafoBOXmM5BPiUojROoAh8PEJUpSsv1dsGfe3FYAGr0K3JUy49NR/ETFgC90Ecab0JGzjOdg3wugVIhUAX+JlE0FVHq4u+Cf7r5ezMKl8vc5q+ajuE3vGz1QR9pvAMZea/pHBQA5WmEVeDsAQq/8mj+Tvjzbr87AVArv5Kj0heajuJHLAD6oQ+n7kVW32E6BwVIHKVxApXgWAEKlzx2d/P7+W6/uzrcIkc1f8R0DL9iATAAfST1b2R0gekcFDCC0syBqp3/JQoiG6UGvwv+mMI3FLX4i8xrPst0DD9jATAAVQgeTT2KjB5tOgsFlIXdAwf5iICCoAulu32TK/UNlwColrsxL/3uqC7xO1gsAAZBFYIlTUvQhXmms1DAlacTJsFigPwli1LD77fpe0MhAGrkHhyVPo2N/8BYAAwSiwByXBylQqASLAbIe4rSIL7ynX5Qnuv3RQDUYCGOan4XG//BYQEwBHwcQK6JoVQIlHsG+JtJbiiP4C83+mFqJmt55z9UvMwM0c4i4EEODCTXWCgVAuUP/pbSSNjY3eDnEK5Gv6xW7sBR6bPY+A8NLy3DxCmC5AlBqUegEqWVB7lnJQ1GDrsbfNM777mp1O3/B5nX/FHTUYKIBcAI6KNNf0EXzjSdgyIkhlKvQMXOD641QEBpRb5ygx+2rv3+1OF6Oar5AtMxgooFwAhx2WAyRlAqCCqwuyjgb3Q02NjzLr9oNo7nLAC1XN53pHi5cIA+2vRTZHBxZKpu8q8EdvcOJMAegrAooNSVn0M0G/zuBMBovUzmtPzIdJSgYwHgEH208cvIyOVQfk/JR2LYXRQkwOmGQWCj1NgXsPsZftCn6DklBkVN8Txu6esMNlYO0qWNH0UnboYtvO8ifxKUBhImun1wYKE5ij0b+wJKd/fsTdxbTIqos98jc1ruMR0lLFgAOEyXNp6ELlmIIi+rFBDloqD8kUCp5yBmMlQIFbp9lBv9KHflD0UcWYyW42RW+knTUcKEBYALdMn06cjZS1HQatNZiIatZ2EQR2lMQRy8cvTFRqlRL2LPBp939cNXIdtQV5wls1rXm44SNvw1domumrYv3tBVyOsE01mIHFcuBGLdPsqfsxDeK4uNvRt5G2zk3VKF/yKZmSlzXmgzHSWMwvpr6gu6cEoStRVPIYdG01mIPFWeomh1+2/5Q3r8v8lHDTZKjbZid+OuKDXm3Rv77l8j9wmAaixFZuJxsmBx0DYiDgwWAC4rLR3cdA+yeCcvHkT9KBcEPT/Qy//3Rnd+rdygd/98uYEH9mzI+TvpP6Ud/X6Po9If59K+7mIB4BFdkvoxuvRS/jgTEfUhBkWtXiZHtvzYdJQoYAHgIX2s/sPosv4PtnB8NRFRd3HJoyb/Hjny2X+ajhIVLAA8pk+kDkenPoICakxnISLyhSTeQFVursxZ+7zpKFHCAsAAffjAsbBrVyCPyaazEBEZVakrYbXPk/kbukxHiRoWAIaUBgem7kJG3206CxGR50pb+f4GRzV/koP9zOCStYaIQOXY9HtQi0vAH34iipK4FlFnnyPzms9j428OewB8QJ9omI8u6wHkwZUDiSjcKrAN4+UYSaWbTUeJOhYAPqGLJo9BvGYpslpvOgsRkStq8Dh0+wI+7/cHFgA+o4+lbsIOPc90DiIix8SgqMHlMrf5a6aj0G4sAHxIlzSegZzcigIqTGchIhqRCulEZe6dMve5h0xHoT2xAPApfSK1H7JYgqwebDoLEdGwVOEZJDNv4WY+/sQCwMdUIXgsdSO69DyOkyWiwLBUUYMfY27LVznK379YAASALkmdjBzuREGrTGchIupXhbRhVPztcvgzT5iOQv1jARAQunTKKBSSDyKjc0xnISLaS2kL339hv87T5OD1GdNxaGAsAAJGlzReioz1Q9jKDYWIyB8SyKOqeL7MXfNb01Fo8FgABJA+PuNg5AqLkcWBprMQUcRVoxk18QUyc+VrpqPQ0LAACChVCJY0XY0sPgeb/45E5LG4FlFtfQdHpr/PgX7BxIYj4PSJ1OHo0nuRx76msxBRBAiApLyAusLbZOaadabj0PCxAAiBnb0B1yGDT0H5b0pELolrEVW4HHNbvsm7/uBjYxEi+nhqJjL6T+Sxv+ksRBQyVXgW1fY7ZFbretNRyBksAEKmtHhQ4xXowhegwu2eiWhk4iigRr+GOS0/5V1/uLAACCl97PDJyOfuQw6Hmc5CRAEkACp1GZLFd8qcZ183HYecxwIg5PTx1GfRpVdwYyEiGrQkOlCLc2VW8+2mo5B7WABEgD4zowbbi3cgq+8wnYWIfCwGRTX+iFo5V1LpnOk45C4WABGij9Ufg1zsdg4SJKK9VOM5WHK6zEs3m45C3mABEEH6WOobyOq3UEDCdBYiMqwCnaixvySzW28wHYW8xQIgojSdqsU2/TOyeCdXEiSKoLjaqMLvMS5/vkxdmzUdh7zHC3/E6SMzU9DCHchpPSf4EEVAade+J1AdP1NmrtxgOg6ZwwKAAAD6eNNZyOB65DHWdBYickmVvIzKwsdk9pp/m45C5rEAoF1UIVja9C1k9KsoStJ0HiJySBLtqLQvxZzWG7iYD5VxpTjaRQQqRzf/P2y2RqEKNyOmRdOZiGgEEshhFC5HYeI4ObL1ejb+1B17AKhP+vCBY2HV/A5ZOZUDBYkCJKZFVOrvdjwhWQAABRJJREFUoB2fkfkbukzHIX/iRZ0GpIumTEI88Qfk5XgWAkQ+llAbFfJXTMycJ4e+0GY6DvkbL+Y0aLo8NQUZ/B8yejS3HSbykbjaSMq9qO06V6av22Q6DgUDL+I0ZPp4w2HIx25GVuexECAyKA4blfJPxO1PyZyWV0zHoWDhxZuGTZdNOxRZ/TUyejwLASIPxaWIJP6O6swFMvP510zHoWDiRZtGTFfUH4BO60bk5GTYnFlC5JoE8kjqnzAh+3k+46eRYgFAjtEHDhmN6uQ1yMvZ3H6YyEEV2IGk3IBi29c5qp+cwgKAHKfXI4EZqf9FTv8HeYwynYcokARAJTYhaf0QL6y+Rs4G1+UgR7EAIFfpo03noIjvIIcDTWchCgQLQAWaUaOXyqyWhabjUHixACBP6NKG2SjErkZO53OcAFEvEiigQv4JK/YFOWrlOtNxKPxYAJCnStsQ48co6EeQR53pPERGCYAEXkcS12Jc7ofclpe8xAKAjNFHGz8AW76JPBq5wiBFShw2EngC1fZXZVbrYtNxKJp40SXj9JkZE9FR/BEKeiZ7BSi0BECFbkaldTOq8V1JpTtMR6JoYwFAvqKPpU5DUb+JHGZzrACFQgJ5VMhiVOEbcnj6CdNxiMpYAJAv6cIpSYxJfhkF/SRnEFDgxKBISgsSxZ+hY7/fyILFBdORiHpiAUC+pyvqD0CX9f9QkDOQx3jTeYh6ZQFIygbE9PeYkPkRV+ojv2MBQIGiy6c1IlP8FgpyMgoYAzWdiCJNAFTgVcTxF1QlLpfDn3nZdCSiwWIBQIGlS6fNQLF4GQpyMvIYazoPRURpoZ6NiOtdqLN+JKn0i6YjEQ0HCwAKBV2SmgItXoqi9S7kcQB7BshRMVUkrBcQ19tQlb2aO/BRGLAAoNDRh6ePhVX8PBTvQ14aUNS46UwUQAlkEdenkLB+j0Lbb7kJD4UNCwAKNVUIlqZOQxGfRFHncxAh9SkGRQKvIC4Potr6FaavWirCviQKLxYAFCn68IFjEau9ADbOQEFSKGo1L/ERJQDi2oYKWYGY/AmFtj/wLp+ihAUARZo+Of0QFIufRAEnoyD1KGi16UzkktK6+9sRw0rE8DdUZX/HZ/kUZSwAiLrRRVMmoTL5CRT17bAxDQWM4T4FARWDIobNiGMF4vIPZNv/JMe9uNV0LCK/4IWNqB+6cEoS46vejULh3bBxJAo4CEWpNJ2LehAAcXQgJs+jAo8irneibeJ/uAIfUd9YABANkT584Fgkak9HUU+CLTNh40AUUMuxBB6JQWHpdsTkBcR0OWKxf6Kw7Z98fk80NCwAiByg1yOBmY0nwLbeiqIeCWAKijIRRa1iYTBMFhQx7EBcXoHYawDrCVTjPjybXi5no2g6HlHQsQAgcpEqYlhWPw856xjAmgXoYSjgAKiOQVGS0Ij/DsagsNAFwRuw8DIsaQWwDBXF/2B2a1oEtumIRGEV7YsPkUGqEDzSMBWWHAUgBcVUqEyCygRAx8BGDWwkAjsI0YINC3lY6ICFrRB5DbBfhKVrILFVsDJP4MjnN3CuPZEZwbywEEWIttbXYYs2wKo4BFo8EMCboLIPIPvAxlio1kFQDRuVEFTARgUUcQgsqFqlXgYR2BAI0GdzW/5aqUFWiCgENuT/t2fvRgCCUBBF70Id9F+UJgYEJhAwzsj4WdsweKeN8z6gG+kCT+QT6QAGiQZuJO3gCnnDWih9VYmTD+HPPvE4PCBQfdfOAAAAAElFTkSuQmCC`

		property int lastPressedIndex: 0
		property int animatedKeyIndex: 0

		model: pagedModel

		Component.onCompleted: {
			console.info(DeviceManager.devices)
		}
	}

	component NameLabel: Label {
		font.bold: true
	}
	component ValueLabel: Label {
	}

	Column {
		id: content
		anchors.left: parent.left
		anchors.leftMargin: 20
		spacing: 10

		GridLayout {
			columns:  2
			width: parent.width
			columnSpacing: 5
			rowSpacing: 5

			NameLabel {
				text: 'Emulator window'
			}
			Switch {
				id: emulatorSwitch
				onToggled: {
					if (checked) {
						if (!childWindow) {
							childWindow = emulatorWindowComponent.createObject()
						}
						childWindow.show()
					} else if (childWindow) {
						childWindow.hide()
					}
					checked = Qt.binding(() => childWindow?.visible)
				}
				Component {
					id: emulatorWindowComponent
					EmulatorWindow {
						x: root.x + root.width
						y: root.y
					}
				}
			}

			NameLabel {
				text: 'ExpectedType:'
			}
			ComboBox {
				textRole: 'text'
				valueRole: 'value'
				onActivated: deck.expectedDeviceType = currentValue
				model: ListModel {}
				Component.onCompleted: {
					const types = [StreamDeck.Any, StreamDeck.Mini, StreamDeck.Original, StreamDeck.MK2, StreamDeck.XL, StreamDeck.Pedal, StreamDeck.Plus];
					for(const t of types)
					{
						model.append({'value': t, 'text': root.deviceTypeToString(t)});
					}
					currentIndex = indexOfValue(deck.expectedDeviceType)
				}
			}

			NameLabel {
				text: 'ConnectedType:'
			}
			ValueLabel {
				text: root.deviceTypeToString(deck.deviceType)
			}

			NameLabel {
				text: 'Description:'
			}
			ValueLabel {
				text: [deck.manufacturer, deck.modelName, deck.serialNumber, deck.firmwareVersion].join(' | ')
			}

			NameLabel {
				text: 'Brightness:'
			}
			Slider {
				from: 0
				to: 100
				value: deck.brightness
				Layout.preferredWidth: 200
				enabled: deck.connected && deck.isOpen && deck.hasDisplay
				onPressedChanged: {
					if (!pressed) {
						deck.brightness = value;
						value = Qt.binding(() => deck.brightness)
					}
				}
			}

			NameLabel {
				text: 'Last pressed:'
			}
			ValueLabel {
				text: deck.lastPressedIndex
			}
		}

		Grid {
			id: grid
			columns:  deck.keyColumns
			spacing: 20
			Repeater {
				model: deck.keyCount
				delegate: Rectangle {
					width: 72
					height: 72
					color: '#303030'
					radius: 5

					Image {
						id: img
						anchors.fill: parent
						property var modelEntry: deck.model && index < deck.model.count ? deck.model.at(index) : undefined

						source: modelEntry ? (modelEntry.image ? modelEntry.imageAsUrl() : modelEntry.imageSource)
								: ''
					}

					Rectangle {
						anchors.fill: parent
						anchors.margins: -3
						color: 'transparent'
						border.color: 'blue'
						border.width: 3
						visible: deck.buttonsState[index]
						radius: 8
					}
				}
			}
		}
		Item {
			id: itemToGrab
			width: 300
			height: 300
			visible: deck.hasDisplay && deck.connected

			property real pressedScale: deck.buttonsState[deck.animatedKeyIndex] ? 0.8 : 1.0
			Behavior on pressedScale {
				NumberAnimation {
					duration: 200;
				}
			}

			function startAnimation() {
				transition.stop()
				timer.stop()
				if (transition.direction === RotationAnimation.Counterclockwise) {
					transition.direction = RotationAnimation.Clockwise
				} else {
					transition.direction = RotationAnimation.Counterclockwise
				}

				transition.start()
				if (deck.hasDisplay && deck.connected) {
					timer.count = 0
					timer.start()
				}
			}

			Rectangle {
				anchors.fill: parent
				color: 'indigo'
			}

			Rectangle {
				id: rect
				width: 200
				height: 200
				anchors.centerIn: parent
				color: 'red'
				antialiasing: true
				scale: itemToGrab.pressedScale

				states: State {
					name: "rotated"
					PropertyChanges { target: rect; rotation: 180 }
				}

				RotationAnimation {
					id: transition
					target: rect
					from: 0
					to: 180
					duration: 1000;
					direction: RotationAnimation.Counterclockwise
				}
			}

			Label {
				anchors.centerIn: parent
				font.pixelSize: 60
				font.bold: true
				text: timer.count
				color: 'white'
				antialiasing: true
			}

			MouseArea {
				anchors.fill: parent
				onClicked: {
					itemToGrab.startAnimation();
				}
			}

			Timer {
				id: timer
				property int count: 0
				interval: 50
				repeat: true
				onTriggered: {
					count++;
					grabber.grab()
					if (!transition.running && itemToGrab.pressedScale >= 1.0) {
						timer.stop();
					}
				}
			}

			ItemGrabber {
				id: grabber
				item: itemToGrab
				targetSize: deck.originalKeyImageSize
			}
		}
	}
}
