/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright (c) 2022 Marvell.
 */

#ifndef _CN10K_ML_DEV_H_
#define _CN10K_ML_DEV_H_

#include <roc_api.h>

/* Marvell OCTEON CN10K ML PMD device name */
#define MLDEV_NAME_CN10K_PMD ml_cn10k

/* Firmware version string length */
#define MLDEV_FIRMWARE_VERSION_LENGTH 32

/* Device alignment size */
#define ML_CN10K_ALIGN_SIZE 128

/* Maximum number of models per device */
#define ML_CN10K_MAX_MODELS 16

/* Maximum number of queue-pairs per device */
#define ML_CN10K_MAX_QP_PER_DEVICE 1

/* Maximum number of descriptors per queue-pair */
#define ML_CN10K_MAX_DESC_PER_QP 1024

/* Maximum number of segments for IO data */
#define ML_CN10K_MAX_SEGMENTS 1

/* ML command timeout in seconds */
#define ML_CN10K_CMD_TIMEOUT 5

/* Poll mode job state */
#define ML_CN10K_POLL_JOB_START	 0
#define ML_CN10K_POLL_JOB_FINISH 1

/* Job types */
enum cn10k_ml_job_type {
	ML_CN10K_JOB_TYPE_MODEL_RUN = 0,
	ML_CN10K_JOB_TYPE_MODEL_STOP,
	ML_CN10K_JOB_TYPE_MODEL_START,
	ML_CN10K_JOB_TYPE_FIRMWARE_LOAD,
	ML_CN10K_JOB_TYPE_FIRMWARE_SELFTEST,
};

/* Device configuration state enum */
enum cn10k_ml_dev_state {
	/* Probed and not configured */
	ML_CN10K_DEV_STATE_PROBED = 0,

	/* Configured */
	ML_CN10K_DEV_STATE_CONFIGURED,

	/* Started */
	ML_CN10K_DEV_STATE_STARTED,

	/* Closed */
	ML_CN10K_DEV_STATE_CLOSED
};

/* Firmware stats */
struct cn10k_ml_fw_stats {
	/* Firmware start cycle */
	uint64_t fw_start;

	/* Firmware end cycle */
	uint64_t fw_end;

	/* Hardware start cycle */
	uint64_t hw_start;

	/* Hardware end cycle */
	uint64_t hw_end;
};

/* Result structure */
struct cn10k_ml_result {
	/* Job error code */
	uint64_t error_code;

	/* Firmware stats */
	struct cn10k_ml_fw_stats stats;

	/* User context pointer */
	void *user_ptr;
};

/* Firmware capability structure */
union cn10k_ml_fw_cap {
	uint64_t u64;

	struct {
		/* CMPC completion support */
		uint64_t cmpc_completions : 1;

		/* Poll mode completion support */
		uint64_t poll_completions : 1;

		/* SSO completion support */
		uint64_t sso_completions : 1;

		/* Support for model side loading */
		uint64_t side_load_model : 1;

		/* Batch execution */
		uint64_t batch_run : 1;

		/* Max number of models to be loaded in parallel */
		uint64_t max_models : 8;

		/* Firmware statistics */
		uint64_t fw_stats : 1;

		/* Hardware statistics */
		uint64_t hw_stats : 1;

		/* Max number of batches */
		uint64_t max_num_batches : 16;

		uint64_t rsvd : 33;
	} s;
};

/* Firmware debug info structure */
struct cn10k_ml_fw_debug {
	/* ACC core 0 debug buffer */
	uint64_t core0_debug_ptr;

	/* ACC core 1 debug buffer */
	uint64_t core1_debug_ptr;

	/* ACC core 0 exception state buffer */
	uint64_t core0_exception_buffer;

	/* ACC core 1 exception state buffer */
	uint64_t core1_exception_buffer;

	/* Debug buffer size per core */
	uint32_t debug_buffer_size;

	/* Exception state dump size */
	uint32_t exception_state_size;
};

/* Job descriptor header (32 bytes) */
struct cn10k_ml_jd_header {
	/* Job completion structure */
	struct ml_jce_s jce;

	/* Model ID */
	uint64_t model_id : 8;

	/* Job type */
	uint64_t job_type : 8;

	/* Flags for fast-path jobs */
	uint64_t fp_flags : 16;

	/* Flags for slow-path jobs */
	uint64_t sp_flags : 16;
	uint64_t rsvd : 16;

	/* Job result pointer */
	uint64_t *result;
};

/* Job descriptor structure */
struct cn10k_ml_jd {
	/* Job descriptor header (32 bytes) */
	struct cn10k_ml_jd_header hdr;

	union {
		struct cn10k_ml_jd_section_fw_load {
			/* Firmware capability structure (8 bytes) */
			union cn10k_ml_fw_cap cap;

			/* Firmware version (32 bytes) */
			uint8_t version[MLDEV_FIRMWARE_VERSION_LENGTH];

			/* Debug capability structure (40 bytes) */
			struct cn10k_ml_fw_debug debug;

			/* Flags to control error handling */
			uint64_t flags;

			uint8_t rsvd[8];
		} fw_load;
	};
};

/* ML firmware structure */
struct cn10k_ml_fw {
	/* Device reference */
	struct cn10k_ml_dev *mldev;

	/* Firmware file path */
	const char *path;

	/* Data buffer */
	uint8_t *data;

	/* Firmware load / handshake request structure */
	struct cn10k_ml_req *req;
};

/* Device private data */
struct cn10k_ml_dev {
	/* Device ROC */
	struct roc_ml roc;

	/* Configuration state */
	enum cn10k_ml_dev_state state;

	/* Firmware */
	struct cn10k_ml_fw fw;

	/* Number of models loaded */
	uint16_t nb_models_loaded;
};

uint64_t cn10k_ml_fw_flags_get(struct cn10k_ml_fw *fw);
int cn10k_ml_fw_load(struct cn10k_ml_dev *mldev);
void cn10k_ml_fw_unload(struct cn10k_ml_dev *mldev);

#endif /* _CN10K_ML_DEV_H_ */
