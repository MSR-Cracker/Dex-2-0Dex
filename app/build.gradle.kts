plugins {
    alias(libs.plugins.android.application)
}

android {
    namespace = "com.msr.nodex"
    compileSdk = 34
    defaultConfig {
        applicationId = "com.msr.nodex"
        minSdk = 21
        targetSdk = 34
        versionCode = 1
        versionName = "1.0"
        multiDexEnabled = false
        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(getDefaultProguardFile("proguard-android-optimize.txt"), "proguard-rules.pro")
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }
    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }
    packaging {
        resources {
            excludes.add("classes.dex")
        }
    }
}

androidComponents {
    onVariants { variant ->

        val capName = variant.name.replaceFirstChar {

            it.uppercase()

        }

        val updateDex = tasks.register("updateDex$capName") {

            dependsOn("mergeDex$capName")

            doLast {

                val dex = file("${layout.buildDirectory.get()}/intermediates/dex/${variant.name}/mergeDex$capName/classes.dex")

                if (dex.exists()) {

                    val hex = dex.readBytes().joinToString("") {
                        "%02x".format(it)
                    }
                    file("src/main/cpp/SafeClass/Dex.hxx").writeText("#define hexdex \"$hex\"")
                    println(">>> Dex.hxx done !!!")
                }
            }
        }
        tasks.configureEach {
            if (name.contains("CMake") && name.contains(capName)) dependsOn(updateDex)
        }
    }
}

dependencies {
    implementation(libs.appcompat)
    implementation(libs.constraintlayout)
    implementation(libs.material)
    testImplementation(libs.junit)
    androidTestImplementation(libs.espresso.core)
    androidTestImplementation(libs.ext.junit)
}
